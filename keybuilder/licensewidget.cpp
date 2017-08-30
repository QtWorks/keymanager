// Application
#include "licensewidget.h"
#include "ui_licensewidget.h"

//-------------------------------------------------------------------------------------------------

LicenseWidget::LicenseWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::LicenseWidget)
{
    ui->setupUi(this);
    ui->questionWidget->setTitle(tr("QUESTION"));
    ui->answerWidget->setTitle(tr("ANSWER"));
    connect(ui->validateButton, &QPushButton::clicked, this, &LicenseWidget::onValidateClicked);
}

//-------------------------------------------------------------------------------------------------

LicenseWidget::~LicenseWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void LicenseWidget::onValidateClicked()
{
    emit validateClicked(ui->questionWidget->getValue(), ui->answerWidget->getValue());
}
