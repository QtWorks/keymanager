// Qt
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QDoubleValidator>

// Application
#include "layoutmgr.h"
#include "ui_layoutmgr.h"
#include "keyblock.h"
#include "collapsiblestack.h"
#include "constants.h"
#include "filepicker.h"
#define MAX_KEY_PER_STACK 5

//-------------------------------------------------------------------------------------------------

LayoutMgr::LayoutMgr(QWidget *parent) : QWidget(parent), ui(new Ui::LayoutMgr),
    m_nKeyBlocks(0)
{
    ui->setupUi(this);
}

//-------------------------------------------------------------------------------------------------

LayoutMgr::~LayoutMgr()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::addKeyBlock(const CXMLNode &xKeyBlock)
{
    QString sBlockName("");
    KeyBlock *pNewKeyBlock = createKeyBlock(xKeyBlock, sBlockName);
    if (sBlockName.isEmpty())
        sBlockName = "NO NAME";

    CollapsibleStack *pTargetStack = nullptr;
    int iStackIndex = m_nKeyBlocks/MAX_KEY_PER_STACK;
    if (iStackIndex < m_vStacks.size())
        pTargetStack = m_vStacks[iStackIndex];
    else
    {
        pTargetStack = new CollapsibleStack(this);
        m_vStacks << pTargetStack;
        ui->horizontalLayout->addWidget(pTargetStack);
        ui->horizontalLayout->setAlignment(pTargetStack, Qt::AlignTop);
    }
    pTargetStack->addPanel(sBlockName, pNewKeyBlock);
    m_nKeyBlocks++;
}

//-------------------------------------------------------------------------------------------------

KeyBlock *LayoutMgr::createKeyBlock(const CXMLNode &xKeyBlock, QString &sBlockName)
{
    // Populate key block with parameters
    QVector<CXMLNode> vParameterNodes = xKeyBlock.getNodesByTagName(TAG_PARAMETER);
    bool bHasParameters = !vParameterNodes.isEmpty();

    // Build key block
    sBlockName = xKeyBlock.attributes()[PROPERTY_NAME];
    KeyBlock *pKeyBlock = bHasParameters ? new KeyBlock() : nullptr;

    foreach (CXMLNode xParameter, vParameterNodes)
    {
        QString sParameterName = xParameter.attributes()[PROPERTY_NAME];
        QString sParameterVariable = xParameter.attributes()[PROPERTY_VARIABLE];
        QString sParameterType = xParameter.attributes()[PROPERTY_TYPE];
        QString sWidgetType = xParameter.attributes()[PROPERTY_UI];
        if (sWidgetType == WIDGET_LINE_EDIT)
        {
            // Label
            QLabel *pLabel = new QLabel(sParameterName, pKeyBlock);
            pKeyBlock->addWidget(pLabel);

            // Widget
            QLineEdit *pLineEdit = new QLineEdit(pKeyBlock);
            pKeyBlock->addWidget(pLineEdit);
            if (sParameterType == PROPERTY_DOUBLE)
            {
                QDoubleValidator *pValidator = new QDoubleValidator(0, 100, 3, pKeyBlock);
                pLineEdit->setValidator(pValidator);
            }

            // Connections
            connect(pLineEdit, &QLineEdit::textChanged, this, &LayoutMgr::onLineEditTextChanged);
            m_hWidgetHash[sParameterVariable] << pLineEdit;
        }
        else
        if (sWidgetType == WIDGET_FILE_PICKER)
        {
            QString sFileExtension = xParameter.attributes()[PROPERTY_FILE_EXTENSION];
            QLabel *pLabel = new QLabel(sParameterName, pKeyBlock);
            pKeyBlock->addWidget(pLabel);
            FilePicker *pFilePicker = new FilePicker(sFileExtension, pKeyBlock);
            pKeyBlock->addWidget(pFilePicker);
            connect(pFilePicker->fileLineEdit(), &QLineEdit::textChanged, this, &LayoutMgr::onLineEditTextChanged);
            m_hWidgetHash[sParameterVariable] << pFilePicker->fileLineEdit();
        }
        else
        if (sWidgetType == WIDGET_RADIO_BUTTON)
        {
            QString sValue = xParameter.attributes()[PROPERTY_VALUE];
            QLabel *pLabel = new QLabel(sParameterName, pKeyBlock);
            pKeyBlock->addWidget(pLabel);
            QRadioButton *pRadioButton = new QRadioButton(pKeyBlock);
            pRadioButton->setAutoExclusive(true);
            pRadioButton->setProperty("userValue", sValue);
            pKeyBlock->addWidget(pRadioButton);
            connect(pRadioButton, &QRadioButton::clicked, this, &LayoutMgr::onRadioButtonClicked);
            m_hWidgetHash[sParameterVariable] << pRadioButton;
        }
    }

    return pKeyBlock;
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onLineEditTextChanged()
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

void LayoutMgr::onRadioButtonClicked()
{
    QRadioButton *pSender = dynamic_cast<QRadioButton *>(sender());
    if ((pSender != nullptr) && (pSender->isChecked()))
    {
        QString sParameterVariable = findAssociatedParameterVariable(pSender);
        if (!sParameterVariable.isEmpty())
        {
            // Retrieve user value
            QString sUserValue = pSender->property("userValue").toString();
            emit parameterValueChanged(sParameterVariable, sUserValue);
        }
    }
}

//-------------------------------------------------------------------------------------------------

QString LayoutMgr::findAssociatedParameterVariable(QWidget *pWidget) const
{
    for (QHash<QString, QVector<QWidget *> >::const_iterator it=m_hWidgetHash.begin(); it!=m_hWidgetHash.end(); ++it)
    {
        if (it.value().contains(pWidget))
            return it.key();
    }
    return QString();
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onExpandAll()
{
    foreach (CollapsibleStack *pStack, m_vStacks)
        pStack->collapseAll();
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onCollapseAll()
{
    foreach (CollapsibleStack *pStack, m_vStacks)
        pStack->expandAll();
}
