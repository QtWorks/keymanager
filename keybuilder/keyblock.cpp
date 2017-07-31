// Qt
#include <QLabel>
#include <QLineEdit>
#include "filepicker.h"
#include <QRadioButton>

// Application
#include "keyblock.h"
#include "ui_keyblock.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

KeyBlock::KeyBlock(const CXMLNode &xKeyBlock, QWidget *parent) : QWidget(parent), ui(new Ui::KeyBlock)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    populateKeyBlock(xKeyBlock);
}

//-------------------------------------------------------------------------------------------------

KeyBlock::~KeyBlock()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void KeyBlock::populateKeyBlock(const CXMLNode &xKeyBlock)
{
    // Populate key block with parameters
    QVector<CXMLNode> vParameterNodes = xKeyBlock.getNodesByTagName(TAG_PARAMETER);
    bool bHasParameters = !vParameterNodes.isEmpty();

    // Build key block
    setName(xKeyBlock.attributes()[PROPERTY_NAME]);
    if (!bHasParameters) {
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
            connect(pLineEdit, &QLineEdit::textChanged, this, &KeyBlock::onLineEditTextChanged);
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
            connect(pFilePicker->fileLineEdit(), &QLineEdit::textChanged, this, &KeyBlock::onLineEditTextChanged);
            m_hWidgetHash[sParameterVariable] << pFilePicker->fileLineEdit();
        }
        else
        if (sWidgetType == WIDGET_RADIO_BUTTON)
        {
            QString sValue = xParameter.attributes()[PROPERTY_VALUE];
            QLabel *pLabel = new QLabel(sParameterName, this);
            addWidget(pLabel);
            QRadioButton *pRadioButton = new QRadioButton(this);
            pRadioButton->setAutoExclusive(true);
            pRadioButton->setProperty("userValue", sValue);
            addWidget(pRadioButton);
            connect(pRadioButton, &QRadioButton::clicked, this, &KeyBlock::onRadioButtonClicked);
            m_hWidgetHash[sParameterVariable] << pRadioButton;
        }
    }
}

//-------------------------------------------------------------------------------------------------

void KeyBlock::onLineEditTextChanged()
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

void KeyBlock::onRadioButtonClicked()
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

QString KeyBlock::findAssociatedParameterVariable(QWidget *pWidget) const
{
    for (QHash<QString, QVector<QWidget *> >::const_iterator it=m_hWidgetHash.begin(); it!=m_hWidgetHash.end(); ++it)
    {
        if (it.value().contains(pWidget))
            return it.key();
    }
    return QString();
}

//-------------------------------------------------------------------------------------------------

void KeyBlock::addWidget(QWidget *pWidget)
{
    ui->verticalLayout->addWidget(pWidget);
    ui->verticalLayout->setAlignment(pWidget, Qt::AlignTop);
}

//-------------------------------------------------------------------------------------------------

void KeyBlock::addWidget(QWidget *pWidget, const QString &sParameterVariable)
{
    addWidget(pWidget);
    m_hWidgetHash[sParameterVariable] << pWidget;
}

//-------------------------------------------------------------------------------------------------

const QString &KeyBlock::name() const
{
    return m_sName;
}

//-------------------------------------------------------------------------------------------------

void KeyBlock::setName(const QString &sName)
{
    m_sName = sName;
}
