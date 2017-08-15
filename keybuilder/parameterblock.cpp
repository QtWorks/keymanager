// Qt
#include <QPainter>
#include <QPaintEvent>
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
#include "widgetfactory.h"

static QVector<QColor> sBlockColors = QVector<QColor>() << Qt::red << Qt::green << Qt::yellow << Qt::blue << Qt::lightGray << Qt::cyan;
//-------------------------------------------------------------------------------------------------

ParameterBlock::ParameterBlock(const CXMLNode &xParameterBlock, CollapsibleBlock *pOwner, Controller *pController, QWidget *parent) : QWidget(parent), ui(new Ui::ParameterBlock),
    m_sName(""), m_bIsEmpty(false), m_sEnabledCondition(""), m_sSelectionVariable(""),
    m_sValue(""), m_bIsExclusive(true), m_bIsEnabled(true),
    m_pOwnerCollapsibleBlock(pOwner), m_pController(pController)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(this, &ParameterBlock::parameterValueChanged, m_pController, &Controller::onParameterValueChanged, Qt::UniqueConnection);
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
    // Set name
    setName(xParameterBlock.attributes()[PROPERTY_NAME].simplified());

    // Set variable
    setSelectionVariable(xParameterBlock.attributes()[PROPERTY_SET_VARIABLE].simplified());

    // Set value
    setValue(xParameterBlock.attributes()[PROPERTY_VALUE].simplified());

    // Set empty state
    QVector<CXMLNode> vParameterNodes = xParameterBlock.getNodesByTagName(TAG_PARAMETER);
    m_bIsEmpty = xParameterBlock.nodes().isEmpty();
    if (m_bIsEmpty)
    {
        setFixedSize(0, 0);
        setVisible(false);
    }

    // Set exclusive state
    QString sExclusive = xParameterBlock.attributes()[PROPERTY_EXCLUSIVE].simplified();
    setExclusive(sExclusive.isEmpty() ? true : (sExclusive == VALUE_TRUE));

    // Process enabled condition
    processEnabledCondition(xParameterBlock);

    // Add widgets
    foreach (CXMLNode xParameter, vParameterNodes)
    {
        BaseWidget *pWidget = m_pController->widgetFactory()->buildWidget(xParameter, this);
        if (pWidget != nullptr)
        {
            addWidget(pWidget);
            m_vWidgets << pWidget;
        }
    }

    // Add child recursively
    addChildRecursively(xParameterBlock);
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::addWidget(BaseWidget *pWidget)
{
    if (pWidget != nullptr)
    {
        ui->verticalLayout->addWidget(pWidget);
        ui->verticalLayout->setAlignment(pWidget, Qt::AlignTop);
    }
}

//-------------------------------------------------------------------------------------------------

int ParameterBlock::nParents() const
{
    int nParents = 0;
    CollapsibleBlock *pOwnerCollapsibleBlock = m_pOwnerCollapsibleBlock;
    while (pOwnerCollapsibleBlock != nullptr)
    {
        nParents++;
        pOwnerCollapsibleBlock = pOwnerCollapsibleBlock->parentBlock();
    }
    return nParents;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::addCollapsibleBlock(CollapsibleBlock *pBlock)
{
    if (pBlock != nullptr)
    {
        ui->verticalLayout->addWidget(pBlock);
        ui->verticalLayout->setAlignment(pBlock, Qt::AlignTop);
    }
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

const QString &ParameterBlock::selectionVariable() const
{
    return m_sSelectionVariable;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::setSelectionVariable(const QString &sParameterVariable)
{
    m_sSelectionVariable = sParameterVariable;
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

CollapsibleBlock *ParameterBlock::ownerCollapsibleBlock() const
{
    return m_pOwnerCollapsibleBlock;
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::setWatchedParameters(const QHash<QString, Parameter *> &hParameters)
{
    m_hWatchedParameters = hParameters;
    for (QHash<QString, Parameter *>::iterator it=m_hWatchedParameters.begin(); it!=m_hWatchedParameters.end(); ++it)
        connect(it.value(), &Parameter::parameterValueChanged, this, &ParameterBlock::onEvaluateEnabledCondition, Qt::UniqueConnection);
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
    foreach (BaseWidget *pWidget, m_vWidgets)
        if (pWidget != nullptr)
            pWidget->applyDefaultValue();
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::addChildRecursively(const CXMLNode &xParameterBlock)
{
    // Parse child blocks
    QVector<CXMLNode> vChildBlocks = xParameterBlock.getNodesByTagName(TAG_BLOCK);
    foreach (CXMLNode xChildBlock, vChildBlocks)
    {
        // Create new collapsible block
        CollapsibleBlock *pNewBlock = new CollapsibleBlock(xChildBlock, m_pController, this);
        if (m_pOwnerCollapsibleBlock != nullptr)
            m_pOwnerCollapsibleBlock->addChildBlock(pNewBlock);

        // Add to own layout
        addCollapsibleBlock(pNewBlock);
    }
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::processEnabledCondition(const CXMLNode &xParameterBlock)
{
    // Read enabled condition
    m_sEnabledCondition = xParameterBlock.attributes()[PROPERTY_ENABLED].simplified();

    if (!m_sEnabledCondition.isEmpty())
    {
        QVector<QString> vVariableNames = ParameterMgr::extractVariableNames(m_sEnabledCondition);
        QHash<QString, Parameter *> hParameters;
        foreach (QString sParameterVariable, vVariableNames)
        {
            Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sParameterVariable);
            if (pParameter != nullptr)
                hParameters[sParameterVariable] = pParameter;
        }
        if (!hParameters.isEmpty() && (hParameters.size() == vVariableNames.size()))
            setWatchedParameters(hParameters);
    }
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    /* TEST ONLY
    QPainter p(this);
    QColor paintColor = sBlockColors[nParents()];
    p.fillRect(e->rect(), paintColor);
    */
}
