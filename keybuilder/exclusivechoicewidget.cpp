// Qt
#include <QRadioButton>

// Application
#include "exclusivechoicewidget.h"
#include "ui_exclusivechoicewidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

ExclusiveChoiceWidget::ExclusiveChoiceWidget(const QStringList &lLabels, const QStringList &lValues, const QString &sLabel, const QString &sDefaultValue, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent) :
    BaseWidget(parent), ui(new Ui::ExclusiveChoiceWidget)
{
    ui->setupUi(this);
    m_sDefaultValue = sDefaultValue;
    if (m_sDefaultValue.isEmpty())
        m_sDefaultValue = PROPERTY_DEFAULT_VALUE;
    m_sAutoScript = sAutoScript;
    m_sEnabledCondition = sEnabledCondition;
    setup(sLabel, lLabels, lValues);
}

//-------------------------------------------------------------------------------------------------

ExclusiveChoiceWidget::~ExclusiveChoiceWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void ExclusiveChoiceWidget::setup(const QString &sLabel, const QStringList &lLabels, const QStringList &lValues)
{
    ui->labelArea->setVisible(!sLabel.simplified().isEmpty());
    ui->labelArea->setText(sLabel.simplified());

    int nItems = qMin(lLabels.size(), lValues.size());
    for (int i=0; i<nItems; i++)
    {
        QRadioButton *pRadioButton = new QRadioButton(lLabels[i], this);
        pRadioButton->setAutoExclusive(true);
        pRadioButton->setProperty(PROPERTY_USER_VALUE, lValues[i]);
        connect(pRadioButton, &QRadioButton::toggled, this, &ExclusiveChoiceWidget::onRadioButtonToggled, Qt::UniqueConnection);
        ui->radioButtonArea->addWidget(pRadioButton);
        m_vRadioButtons << pRadioButton;
    }
}

//-------------------------------------------------------------------------------------------------

void ExclusiveChoiceWidget::applyDefaultValue()
{
    applyValue(m_sDefaultValue);
}

//-------------------------------------------------------------------------------------------------

void ExclusiveChoiceWidget::applyValue(const QString &sValue)
{
    if (!sValue.isEmpty())
    {
        bool bMatch = false;
        foreach (QRadioButton *pRadioButton, m_vRadioButtons)
        {
            QString sUserValue = pRadioButton->property(PROPERTY_USER_VALUE).toString();
            if (sUserValue == sValue)
            {
                bMatch = true;
                pRadioButton->setChecked(true);
                break;
            }
        }
        if (!bMatch && !m_vRadioButtons.isEmpty())
            m_vRadioButtons.first()->setChecked(true);
    }
    else
    if (!m_vRadioButtons.isEmpty())
        m_vRadioButtons.first()->setChecked(true);
}

//-------------------------------------------------------------------------------------------------

void ExclusiveChoiceWidget::onRadioButtonToggled(bool bClicked)
{
    QRadioButton *pSender = dynamic_cast<QRadioButton *>(sender());
    if (pSender && bClicked)
    {
        QString sUserValue = pSender->property(PROPERTY_USER_VALUE).toString();
        emit parameterValueChanged(m_sParameterVariable, sUserValue);
    }
}
