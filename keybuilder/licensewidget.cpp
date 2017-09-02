// Qt
#include <QPainter>

// Application
#include "licensewidget.h"
#include "ui_licensewidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

LicenseWidget::LicenseWidget(QWidget *parent) : DescriptionTaggedWidget(parent),
    ui(new Ui::LicenseWidget)
{
    // Setup UI
    ui->setupUi(this);
    ui->questionWidget->setTitle(tr("QUESTION"));
    ui->questionWidget->showClipBoardButton(true);
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

//-------------------------------------------------------------------------------------------------

void LicenseWidget::setQuestion(const QString &sQuestion)
{
    ui->questionWidget->setValue(sQuestion);
    ui->questionWidget->setReadOnly(true);
}

