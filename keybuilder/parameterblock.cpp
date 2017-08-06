// Qt
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QDebug>

// Application
#include "controller.h"
#include "parameterblock.h"
#include "ui_parameterblock.h"
#include "constants.h"
#include "collapsibleblock.h"
#include "filepickerwidget.h"
#include "dxforstlfilepicker.h"
#include "doubletripletwidget.h"
#include "layoutmgr.h"
#include "exclusivechoicewidget.h"
#include "lineeditwidget.h"
#include "genericparametertable.h"
#include "parametermgr.h"
#include "basewidget.h"

//-------------------------------------------------------------------------------------------------

ParameterBlock::ParameterBlock(const CXMLNode &xParameterBlock, Controller *pController, bool bRecurse, QWidget *parent) : QWidget(parent), ui(new Ui::ParameterBlock),
    m_sName(""), m_bIsEmpty(false), m_sEnabledCondition(""), m_sVariableName(""),
    m_sValue(""), m_bIsExclusive(true), m_bIsEnabled(true),
    m_pParentBlock(nullptr), m_pController(pController)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(this, &ParameterBlock::parameterValueChanged, m_pController, &Controller::onParameterValueChanged);
    populateParameterBlock(xParameterBlock, bRecurse);
}

//-------------------------------------------------------------------------------------------------

ParameterBlock::~ParameterBlock()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::populateParameterBlock(const CXMLNode &xParameterBlock, bool bRecurse)
{
    // Set name
    setName(xParameterBlock.attributes()[PROPERTY_NAME]);

    // Set variable
    setVariable(xParameterBlock.attributes()[PROPERTY_VARIABLE]);

    // Set value
    setValue(xParameterBlock.attributes()[PROPERTY_VALUE]);

    // Populate parameter block with parameters
    QVector<CXMLNode> vParameterNodes = xParameterBlock.getNodesByTagName(TAG_PARAMETER);
    m_bIsEmpty = xParameterBlock.nodes().isEmpty();
    QString sExclusive = xParameterBlock.attributes()[PROPERTY_EXCLUSIVE].simplified();
    m_bIsExclusive = sExclusive.isEmpty() ? true : (sExclusive == VALUE_TRUE);

    // Read enabled condition
    m_sEnabledCondition = xParameterBlock.attributes()[PROPERTY_ENABLED];

    if (!m_sEnabledCondition.isEmpty())
    {
        QVector<QString> vVariableNames = ParameterMgr::extractVariableNames(m_sEnabledCondition);
        QHash<QString, Parameter *> hParameters;
        foreach (QString sParameterVariableName, vVariableNames)
        {
            Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sParameterVariableName);
            if (pParameter != nullptr)
                hParameters[sParameterVariableName] = pParameter;
        }
        if (!hParameters.isEmpty() && (hParameters.size() == vVariableNames.size())) {
            setWatchedParameters(hParameters);
        }
    }

    if (m_bIsEmpty)
    {
        setFixedSize(0, 0);
        setVisible(false);
    }

    foreach (CXMLNode xParameter, vParameterNodes)
    {
        QString sParameterName = xParameter.attributes()[PROPERTY_NAME].simplified();
        QString sParameterVariable = xParameter.attributes()[PROPERTY_VARIABLE].simplified();
        QString sParameterType = xParameter.attributes()[PROPERTY_TYPE].simplified();
        QString sWidgetType = xParameter.attributes()[PROPERTY_UI].simplified();
        QString sDefaultValue = xParameter.attributes()[PROPERTY_DEFAULT].simplified();
        QString sAuto = xParameter.attributes()[PROPERTY_AUTO].simplified();

        // Parameter with no UI
        if (sWidgetType.isEmpty())
        {
            Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sParameterVariable);
            if (pParameter != nullptr)
                pParameter->setValue(sDefaultValue);
        }
        else
        if (sWidgetType == WIDGET_LINE_EDIT)
        {
            LineEditWidget *pLineEdit = new LineEditWidget(sParameterName, sDefaultValue, sAuto, this);
            pLineEdit->setParameterMgr(m_pController->parameterMgr());
            if (sParameterType == PROPERTY_DOUBLE)
            {
                QDoubleValidator *pValidator = new QDoubleValidator(0, 100, 3, this);
                pLineEdit->setValidator(pValidator);
            }
            connect(pLineEdit, &LineEditWidget::valueChanged, this, &ParameterBlock::onLineEditTextChanged);
            addWidget(pLineEdit, sParameterVariable);
            pLineEdit->applyDefaultValue();

            if (!sAuto.isEmpty())
            {
                QVector<QString> vVariableNames = ParameterMgr::extractVariableNames(sAuto);
                QHash<QString, Parameter *> hParameters;
                foreach (QString sParameterVariableName, vVariableNames)
                {
                    Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sParameterVariableName);
                    if (pParameter != nullptr)
                        hParameters[sParameterVariableName] = pParameter;
                }
                if (!hParameters.isEmpty() && (hParameters.size() == vVariableNames.size()))
                    pLineEdit->setWatchedParameters(hParameters);
            }
        }
        else
        if (sWidgetType == WIDGET_FILE_PICKER)
        {
            QString sFileExtension = xParameter.attributes()[PROPERTY_FILE_EXTENSION];
            FilePickerWidget *pFilePickerWidget = new FilePickerWidget(sParameterName, sFileExtension, sDefaultValue, this);
            connect(pFilePickerWidget, &FilePickerWidget::textChanged, this, &ParameterBlock::onFilePickerTextChanged);
            addWidget(pFilePickerWidget, sParameterVariable);
            pFilePickerWidget->applyDefaultValue();
        }
        else
        if (sWidgetType == WIDGET_DXF_OR_STL_FILE_PICKER)
        {
            QString sParameterSTLVariable = xParameter.attributes()[PROPERTY_STL_VARIABLE].simplified();
            QString sParameterDXFVariable = xParameter.attributes()[PROPERTY_DXF_VARIABLE].simplified();
            DXForSTLFilePicker *pFilePickerWidget = new DXForSTLFilePicker(sDefaultValue, sParameterSTLVariable, sParameterDXFVariable, this);
            connect(pFilePickerWidget, &DXForSTLFilePicker::dxfSelected, this, &ParameterBlock::onDXFSelected);
            connect(pFilePickerWidget, &DXForSTLFilePicker::stlSelected, this, &ParameterBlock::onSTLSelected);
            addWidget(pFilePickerWidget, sParameterVariable);
            pFilePickerWidget->applyDefaultValue();
        }
        else
        if (sWidgetType == WIDGET_EXCLUSIVE_CHOICE)
        {
            QString sLabels = xParameter.attributes()[PROPERTY_LABELS].simplified();
            QString sValues = xParameter.attributes()[PROPERTY_VALUES].simplified();

            if (!sLabels.isEmpty() && !sValues.isEmpty())
            {
                ExclusiveChoiceWidget *pExclusiveChoiceWidet = new ExclusiveChoiceWidget(sLabels.split(","), sValues.split(","), sParameterName, sDefaultValue, this);
                connect(pExclusiveChoiceWidet, &ExclusiveChoiceWidget::selectionChanged, this, &ParameterBlock::onRadioButtonClicked);
                addWidget(pExclusiveChoiceWidet, sParameterVariable);
                pExclusiveChoiceWidet->applyDefaultValue();
            }
        }
        else
        if (sWidgetType == WIDGET_DOUBLE_TRIPLET)
        {
            DoubleTripletWidget *pTriplet = new DoubleTripletWidget(sParameterName, sDefaultValue, this);
            connect(pTriplet, &DoubleTripletWidget::valueChanged, this, &ParameterBlock::onLineEditTripletValueChanged);
            addWidget(pTriplet, sParameterVariable);
            pTriplet->applyDefaultValue();
        }
        else
        if (sWidgetType == WIDGET_GENERIC_PARAMETER_TABLE)
        {
            QString sColumnLabels = xParameter.attributes()[PROPERTY_COLUMN_LABELS].simplified();
            QString sColumnVariables = xParameter.attributes()[PROPERTY_COLUMN_VARIABLES].simplified();

            if (!sColumnLabels.isEmpty() && !sColumnVariables.isEmpty())
            {
                QString sTargetRow = xParameter.attributes()[PROPERTY_TARGET_ROW].simplified();
                int nRows = xParameter.attributes()[PROPERTY_NROWS].toInt();
                QString sTargetVariable = xParameter.attributes()[PROPERTY_TARGET_VARIABLE];
                QString sVariableMethod = xParameter.attributes()[PROPERTY_VARIABLE_METHOD];
                QStringList lDefaultValues;
                if (!sDefaultValue.isEmpty())
                    lDefaultValues = sDefaultValue.split(",");
                GenericParameterTable *pGenericParameterTable = new GenericParameterTable(sColumnLabels.split(","), sColumnVariables.split(","), lDefaultValues, sTargetRow, nRows, sTargetVariable, sVariableMethod, this);
                connect(pGenericParameterTable, &GenericParameterTable::parameterValueChanged, this, &ParameterBlock::parameterValueChanged);
                addWidget(pGenericParameterTable, sParameterVariable);
                pGenericParameterTable->applyDefaultValue();
            }
        }
    }

    if (bRecurse)
    {
        // Parse child blocks
        QVector<CXMLNode> vChildBlocks = xParameterBlock.getNodesByTagName(TAG_BLOCK);
        foreach (CXMLNode xChildBlock, vChildBlocks)
        {
            // Get child block name
            QString sChildBlockName = xChildBlock.attributes()[PROPERTY_NAME];

            // Build new parameter block
            ParameterBlock *pChildParameterBlock = new ParameterBlock(xChildBlock, m_pController);
            pChildParameterBlock->setParentBlock(this);
            connect(pChildParameterBlock, &ParameterBlock::parameterValueChanged, m_pController, &Controller::onParameterValueChanged);

            // Create new collapsible block
            CollapsibleBlock *pNewBlock = new CollapsibleBlock(pChildParameterBlock, sChildBlockName, pChildParameterBlock->isEmpty(), this);

            // Add to own layout
            addWidget(pNewBlock);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::onLineEditTextChanged()
{
    LineEditWidget *pSender = dynamic_cast<LineEditWidget *>(sender());
    if (pSender != nullptr)
    {
        QString sParameterVariable = findAssociatedParameterVariable(pSender);
        if (!sParameterVariable.isEmpty())
        {
            emit parameterValueChanged(sParameterVariable, pSender->value());
        }
    }
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::onFilePickerTextChanged()
{
    FilePickerWidget *pSender = dynamic_cast<FilePickerWidget *>(sender());
    if (pSender != nullptr)
    {
        QString sParameterVariable = findAssociatedParameterVariable(pSender);
        if (!sParameterVariable.isEmpty())
        {
            emit parameterValueChanged(sParameterVariable, pSender->value());
        }
    }
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::onDXFSelected()
{
    DXForSTLFilePicker *pSender = dynamic_cast<DXForSTLFilePicker *>(sender());
    if (pSender != nullptr)
    {
        emit parameterValueChanged(pSender->dxfVariable(), pSender->dxfValue());
    }
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::onSTLSelected()
{
    DXForSTLFilePicker *pSender = dynamic_cast<DXForSTLFilePicker *>(sender());
    if (pSender != nullptr)
    {
        emit parameterValueChanged(pSender->stlVariable(), pSender->stlValue());
    }
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::onRadioButtonClicked(const QString &sSelection)
{
    ExclusiveChoiceWidget *pSender = dynamic_cast<ExclusiveChoiceWidget *>(sender());
    if (pSender != nullptr)
    {
        QString sParameterVariable = findAssociatedParameterVariable(pSender);
        if (!sParameterVariable.isEmpty())
        {
            // Notify
            emit parameterValueChanged(sParameterVariable, sSelection);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::onLineEditTripletValueChanged()
{
    DoubleTripletWidget *pSender = dynamic_cast<DoubleTripletWidget *>(sender());
    if (pSender != nullptr)
    {
        QString sParameterVariable = findAssociatedParameterVariable(pSender);
        if (!sParameterVariable.isEmpty())
        {
            // Notify
            emit parameterValueChanged(sParameterVariable, pSender->value());
        }
    }
}

//-------------------------------------------------------------------------------------------------

QString ParameterBlock::findAssociatedParameterVariable(BaseWidget *pWidget) const
{
    for (QHash<QString, BaseWidget *>::const_iterator it=m_hWidgetHash.begin(); it!=m_hWidgetHash.end(); ++it)
    {
        if (it.value() == pWidget)
            return it.key();
    }
    return QString();
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::addWidget(BaseWidget *pWidget)
{
    ui->verticalLayout->addWidget(pWidget);
    ui->verticalLayout->setAlignment(pWidget, Qt::AlignTop);
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::addWidget(QWidget *pWidget)
{
    ui->verticalLayout->addWidget(pWidget);
    ui->verticalLayout->setAlignment(pWidget, Qt::AlignTop);
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::addWidget(BaseWidget *pWidget, const QString &sParameterVariable)
{
    addWidget(pWidget);
    m_hWidgetHash[sParameterVariable] = pWidget;
}

//-------------------------------------------------------------------------------------------------

const QString &ParameterBlock::name() const
{
    return m_sName;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::setName(const QString &sName)
{
    m_sName = sName;
}

//-------------------------------------------------------------------------------------------------

const QString &ParameterBlock::variable() const
{
    return m_sVariableName;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::setVariable(const QString &sVariableName)
{
    m_sVariableName = sVariableName;
}

//-------------------------------------------------------------------------------------------------

const QString &ParameterBlock::value() const
{
    return m_sValue;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::setValue(const QString &sValue)
{
    m_sValue = sValue;
}

//-------------------------------------------------------------------------------------------------

bool ParameterBlock::isEmpty() const
{
    return m_bIsEmpty;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::setEnabled(bool bEnabled)
{
    m_bIsEnabled = bEnabled;
    QWidget::setEnabled(bEnabled);
    CollapsibleBlock *pOwnerCollapsibleBlock = dynamic_cast<CollapsibleBlock *>(parentWidget());
    if (pOwnerCollapsibleBlock != nullptr)
        pOwnerCollapsibleBlock->onUpdateEnabledState(bEnabled);
}

//-------------------------------------------------------------------------------------------------

bool ParameterBlock::isEnabled() const
{
    return m_bIsEnabled;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::setExclusive(bool bIsExclusive)
{
    m_bIsExclusive = bIsExclusive;
}

//-------------------------------------------------------------------------------------------------

bool ParameterBlock::isExclusive() const
{
    return m_bIsExclusive;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::setParentBlock(ParameterBlock *pParentBlock)
{
    m_pParentBlock = pParentBlock;
}

//-------------------------------------------------------------------------------------------------

ParameterBlock *ParameterBlock::parentBlock() const
{
    return m_pParentBlock;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::setWatchedParameters(const QHash<QString, Parameter *> &hParameters)
{
    m_hWatchedParameters = hParameters;
    for (QHash<QString, Parameter *>::iterator it=m_hWatchedParameters.begin(); it!=m_hWatchedParameters.end(); ++it)
        connect(it.value(), &Parameter::parameterValueChanged, this, &ParameterBlock::onEvaluateEnabledCondition);
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::onEvaluateEnabledCondition()
{
    bool bSuccess = true;
    bool bEnabled = m_pController->parameterMgr()->evaluateEnabledCondition(m_sEnabledCondition, bSuccess);
    if (bSuccess)
        setEnabled(bEnabled);
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::clearAll()
{
    for (QHash<QString, BaseWidget *>::iterator it=m_hWidgetHash.begin(); it!=m_hWidgetHash.end(); ++it)
        if (it.value() != nullptr)
            it.value()->applyDefaultValue();
}
