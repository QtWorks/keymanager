// Qt
#include <QRadioButton>

// Application
#include "exclusivechoicewidget.h"
#include "ui_exclusivechoicewidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

ExclusiveChoiceWidget::ExclusiveChoiceWidget(Controller *pController, const QStringList &lLabels, const QStringList &lValues, const QString &sLabel, const QString &sDefaultValue, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent) :
    BaseWidget(pController, parent), ui(new Ui::ExclusiveChoiceWidget)
{
    ui->setupUi(this);

    // Set default value
    setDefaultValue(sDefaultValue);

    // Set auto script
    setAutoScript(sAutoScript);

    // Set enabled condition
    setEnabledCondition(sEnabledCondition);

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
    ui->labelArea->setVisible(!sLabel.isEmpty());
    ui->labelArea->setText(sLabel);

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
    applyValue(defaultValue());
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
        emit parameterValueChanged(parameterVariable(), sUserValue);
    }
}
