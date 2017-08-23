// Qt
#include <QRadioButton>

// Application
#include "yesnowidget.h"
#include "ui_yesnowidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

YesNoWidget::YesNoWidget(Controller *pController, const QString &sLabel, const QString &sDefaultValue, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent) :
    BaseWidget(pController, parent), ui(new Ui::YesNoWidget)
{
    ui->setupUi(this);

    // Set default value
    setDefaultValue(sDefaultValue);

    // Set auto script
    setAutoScript(sAutoScript);

    // Set enabled condition
    setEnabledCondition(sEnabledCondition);

    ui->label->setText(sLabel);
    connect(ui->yesRadioButton, &QRadioButton::toggled, this, &YesNoWidget::onRadioButtonToggled, Qt::UniqueConnection);
    ui->yesRadioButton->setProperty(PROPERTY_USER_VALUE, VALUE_YES);
    connect(ui->noRadioButton, &QRadioButton::toggled, this, &YesNoWidget::onRadioButtonToggled, Qt::UniqueConnection);
    ui->noRadioButton->setProperty(PROPERTY_USER_VALUE, VALUE_NO);
}

//-------------------------------------------------------------------------------------------------

YesNoWidget::~YesNoWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void YesNoWidget::applyDefaultValue()
{
    applyValue(defaultValue());
}

//-------------------------------------------------------------------------------------------------

void YesNoWidget::applyValue(const QString &sValue)
{
    if (sValue == VALUE_YES)
        ui->yesRadioButton->setChecked(true);
    else
        ui->noRadioButton->setChecked(true);
}

//-------------------------------------------------------------------------------------------------

void YesNoWidget::onRadioButtonToggled(bool bClicked)
{
    QRadioButton *pSender = dynamic_cast<QRadioButton *>(sender());
    if (pSender && bClicked)
    {
        QString sUserValue = pSender->property(PROPERTY_USER_VALUE).toString();
        emit parameterValueChanged(parameterVariable(), sUserValue);
    }
}
