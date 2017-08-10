// Qt
#include <QRadioButton>

// Application
#include "yesnowidget.h"
#include "ui_yesnowidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

YesNoWidget::YesNoWidget(const QString &sLabel, const QString &sDefaultValue, QWidget *parent) :
    BaseWidget(parent), ui(new Ui::YesNoWidget)
{
    ui->setupUi(this);
    m_sDefaultValue = sDefaultValue;
    ui->label->setText(sLabel);
    connect(ui->yesRadioButton, &QRadioButton::toggled, this, &YesNoWidget::onRadioButtonToggled);
    ui->yesRadioButton->setProperty(PROPERTY_USER_VALUE, PROPERTY_YES);
    connect(ui->noRadioButton, &QRadioButton::toggled, this, &YesNoWidget::onRadioButtonToggled);
    ui->noRadioButton->setProperty(PROPERTY_USER_VALUE, PROPERTY_NO);
}

//-------------------------------------------------------------------------------------------------

YesNoWidget::~YesNoWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void YesNoWidget::applyDefaultValue()
{
    if (m_sDefaultValue == PROPERTY_YES)
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
        emit parameterValueChanged(m_sParameterVariable, sUserValue);
    }
}
