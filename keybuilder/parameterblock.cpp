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
#include "doubletripletwidget.h"
#include "layoutmgr.h"
#include "exclusivechoicewidget.h"
#include "lineeditwidget.h"
#include "levertablewidget.h"

//-------------------------------------------------------------------------------------------------

ParameterBlock::ParameterBlock(const CXMLNode &xParameterBlock, LayoutMgr *pLayoutMgr, QWidget *parent) : QWidget(parent), ui(new Ui::ParameterBlock),
    m_bIsEmpty(false), m_pLayoutMgr(pLayoutMgr)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    populateParameterBlock(xParameterBlock);
}

//-------------------------------------------------------------------------------------------------

ParameterBlock::~ParameterBlock()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::populateParameterBlock(const CXMLNode &xParameterBlock)
{
    // Populate parameter block with parameters
    QVector<CXMLNode> vParameterNodes = xParameterBlock.getNodesByTagName(TAG_PARAMETER);
    m_bIsEmpty = xParameterBlock.nodes().isEmpty();

    // Build parameter block
    setName(xParameterBlock.attributes()[PROPERTY_NAME]);

    if (m_bIsEmpty) {
        setFixedSize(0, 0);
        setVisible(false);
    }

    foreach (CXMLNode xParameter, vParameterNodes)
    {
        QString sParameterName = xParameter.attributes()[PROPERTY_NAME];
        QString sParameterVariable = xParameter.attributes()[PROPERTY_VARIABLE];
        QString sParameterType = xParameter.attributes()[PROPERTY_TYPE];
        QString sWidgetType = xParameter.attributes()[PROPERTY_UI];
        if (sWidgetType == WIDGET_LINE_EDIT)
        {
            LineEditWidget *pLineEdit = new LineEditWidget(sParameterName, this);
            if (sParameterType == PROPERTY_DOUBLE)
            {
                QDoubleValidator *pValidator = new QDoubleValidator(0, 100, 3, this);
                pLineEdit->setValidator(pValidator);
            }
            connect(pLineEdit, &LineEditWidget::valueChanged, this, &ParameterBlock::onLineEditTextChanged);
            addWidget(pLineEdit, sParameterVariable);
        }
        else
        if (sWidgetType == WIDGET_FILE_PICKER)
        {
            QString sFileExtension = xParameter.attributes()[PROPERTY_FILE_EXTENSION];
            FilePickerWidget *pFilePickerWidget = new FilePickerWidget(sParameterName, sFileExtension, this);
            connect(pFilePickerWidget, &FilePickerWidget::textChanged, this, &ParameterBlock::onFilePickerTextChanged);
            addWidget(pFilePickerWidget, sParameterVariable);
        }
        else
        if (sWidgetType == WIDGET_EXCLUSIVE_CHOICE)
        {
            QString sLabels = xParameter.attributes()[PROPERTY_LABELS].simplified();
            QString sValues = xParameter.attributes()[PROPERTY_VALUES].simplified();
            ExclusiveChoiceWidget *pExclusiveChoiceWidet = new ExclusiveChoiceWidget(sLabels.split(","), sValues.split(","), sParameterName, this);
            connect(pExclusiveChoiceWidet, &ExclusiveChoiceWidget::selectionChanged, this, &ParameterBlock::onRadioButtonClicked);
            addWidget(pExclusiveChoiceWidet, sParameterVariable);
        }
        else
        if (sWidgetType == WIDGET_DOUBLE_TRIPLET)
        {
            DoubleTripletWidget *pTriplet = new DoubleTripletWidget(sParameterName, this);
            connect(pTriplet, &DoubleTripletWidget::valueChanged, this, &ParameterBlock::onLineEditTripletValueChanged);
            addWidget(pTriplet, sParameterVariable);
        }
        else
        if (sWidgetType == WIDGET_LEVER_TABLE)
        {
            QString sColumnLabels = xParameter.attributes()[PROPERTY_COLUMN_LABELS].simplified();
            QString sColumnVariables = xParameter.attributes()[PROPERTY_COLUMN_VARIABLES].simplified();
            QString sTargetRow = xParameter.attributes()[PROPERTY_TARGET_ROW].simplified();
            int nRows = xParameter.attributes()[PROPERTY_NROWS].toInt();
            QString sTargetVariable = xParameter.attributes()[PROPERTY_TARGET_VARIABLE];
            LeverTableWidget *pLeverTableWidget = new LeverTableWidget(sColumnLabels.split(","), sColumnVariables.split(","), sTargetRow, nRows, sTargetVariable);
            connect(pLeverTableWidget, &LeverTableWidget::parameterValueChanged, this, &ParameterBlock::parameterValueChanged);
            addWidget(pLeverTableWidget, sParameterVariable);
        }
    }

    // Parse child blocks
    QVector<CXMLNode> vChildBlocks = xParameterBlock.getNodesByTagName(TAG_BLOCK);
    foreach (CXMLNode xChildBlock, vChildBlocks)
    {
        // Get child block name
        QString sChildBlockName = xChildBlock.attributes()[PROPERTY_NAME];

        // Build new parameter block
        ParameterBlock *pChildParameterBlock = new ParameterBlock(xChildBlock, m_pLayoutMgr);
        connect(pChildParameterBlock, &ParameterBlock::parameterValueChanged, m_pLayoutMgr->controller(), &Controller::onParameterValueChanged);

        // Create new collapsible block
        CollapsibleBlock *pNewBlock = new CollapsibleBlock(pChildParameterBlock, sChildBlockName, pChildParameterBlock->isEmpty(), this);

        // Add to own layout
        addWidget(pNewBlock);
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

QString ParameterBlock::findAssociatedParameterVariable(QWidget *pWidget) const
{
    for (QHash<QString, QWidget *>::const_iterator it=m_hWidgetHash.begin(); it!=m_hWidgetHash.end(); ++it)
    {
        if (it.value() == pWidget)
            return it.key();
    }
    return QString();
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::addWidget(QWidget *pWidget)
{
    ui->verticalLayout->addWidget(pWidget);
    ui->verticalLayout->setAlignment(pWidget, Qt::AlignTop);
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::addWidget(QWidget *pWidget, const QString &sParameterVariable)
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

bool ParameterBlock::isEmpty() const
{
    return m_bIsEmpty;
}
