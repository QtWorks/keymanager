// Qt
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>

// Application
#include "parameterblock.h"
#include "ui_parameterblock.h"
#include "constants.h"
#include "collapsibleblock.h"
#include "filepicker.h"
#include "lineedittriplet.h"
#include "layoutmgr.h"
#include "exclusivechoicewidget.h"

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
            // Label
            QLabel *pLabel = new QLabel(sParameterName, this);
            addWidget(pLabel);

            // Widget
            QLineEdit *pLineEdit = new QLineEdit(this);
            addWidget(pLineEdit);
            if (sParameterType == PROPERTY_DOUBLE)
            {
                QDoubleValidator *pValidator = new QDoubleValidator(0, 100, 3, this);
                pLineEdit->setValidator(pValidator);
            }

            // Connections
            connect(pLineEdit, &QLineEdit::textChanged, this, &ParameterBlock::onLineEditTextChanged);
            m_hWidgetHash[sParameterVariable] << pLineEdit;
        }
        else
        if (sWidgetType == WIDGET_FILE_PICKER)
        {
            QString sFileExtension = xParameter.attributes()[PROPERTY_FILE_EXTENSION];
            QLabel *pLabel = new QLabel(sParameterName, this);
            addWidget(pLabel);
            FilePicker *pFilePicker = new FilePicker(sFileExtension, this);
            addWidget(pFilePicker);
            connect(pFilePicker->fileLineEdit(), &QLineEdit::textChanged, this, &ParameterBlock::onLineEditTextChanged);
            m_hWidgetHash[sParameterVariable] << pFilePicker->fileLineEdit();
        }
        else
        if (sWidgetType == WIDGET_EXCLUSIVE_CHOICE)
        {
            QString sLabels = xParameter.attributes()[PROPERTY_LABELS].simplified();
            QString sValues = xParameter.attributes()[PROPERTY_VALUES].simplified();
            ExclusiveChoiceWidget *pExclusiveChoiceWidet = new ExclusiveChoiceWidget(sLabels.split(",").toVector(), sValues.split(",").toVector(), "", this);
            addWidget(pExclusiveChoiceWidet);
            connect(pExclusiveChoiceWidet, &ExclusiveChoiceWidget::selectionChanged, this, &ParameterBlock::onRadioButtonClicked);
            m_hWidgetHash[sParameterVariable] << pExclusiveChoiceWidet;
        }
        else
        if (sWidgetType == WIDGET_DOUBLE_TRIPLET)
        {
            QLabel *pLabel = new QLabel(sParameterName, this);
            addWidget(pLabel);
            LineEditTriplet *pTriplet = new LineEditTriplet(this);
            addWidget(pTriplet);
            connect(pTriplet, &LineEditTriplet::valueChanged, this, &ParameterBlock::onLineEditTripletValueChanged);
            m_hWidgetHash[sParameterVariable] << pTriplet;
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
        connect(pChildParameterBlock, &ParameterBlock::parameterValueChanged, m_pLayoutMgr, &LayoutMgr::parameterValueChanged);

        // Create new collapsible block
        CollapsibleBlock *pNewBlock = new CollapsibleBlock(pChildParameterBlock, sChildBlockName, pChildParameterBlock->isEmpty(), this);
        connect(pNewBlock, &CollapsibleBlock::blockSelected, pChildParameterBlock, &ParameterBlock::onSelectMe);

        // Add to own layout
        addWidget(pNewBlock);
    }
}

//-------------------------------------------------------------------------------------------------

void ParameterBlock::onLineEditTextChanged()
{
    QLineEdit *pSender = dynamic_cast<QLineEdit *>(sender());
    if (pSender != nullptr)
    {
        QString sParameterVariable = findAssociatedParameterVariable(pSender);
        if (!sParameterVariable.isEmpty())
        {
            emit parameterValueChanged(sParameterVariable, pSender->text());
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
    LineEditTriplet *pSender = dynamic_cast<LineEditTriplet *>(sender());
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

void ParameterBlock::onSelectMe()
{
    emit parameterValueChanged(PARAMETER_TYPE_OF_KEY, m_sName);
}

//-------------------------------------------------------------------------------------------------

QString ParameterBlock::findAssociatedParameterVariable(QWidget *pWidget) const
{
    for (QHash<QString, QVector<QWidget *> >::const_iterator it=m_hWidgetHash.begin(); it!=m_hWidgetHash.end(); ++it)
    {
        if (it.value().contains(pWidget))
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
    m_hWidgetHash[sParameterVariable] << pWidget;
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
