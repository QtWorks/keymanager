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
    ui->scriptMissingErrorLabel->hide();
    ui->questionWidget->setBlockCount(20);
    ui->questionWidget->setReadOnly(true);
    ui->questionWidget->setTitle(tr("QUESTION (READ ONLY)"));
    ui->questionWidget->showClipBoardButton(true);
    ui->answerWidget->setBlockCount(4);
    ui->answerWidget->setTitle(tr("ANSWER (CTRL+V)"));
    connect(ui->validateButton, &QPushButton::clicked, this, &LicenseWidget::onValidateClicked, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

LicenseWidget::~LicenseWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void LicenseWidget::onValidateClicked()
{
    emit validateClicked(ui->answerWidget->getValue());
}

//-------------------------------------------------------------------------------------------------

void LicenseWidget::setQuestion(const QString &sQuestion)
{
    ui->questionWidget->setValue(sQuestion);
}

//-------------------------------------------------------------------------------------------------

void LicenseWidget::setAnswer(const QString &sAnswer)
{
    ui->answerWidget->setValue(sAnswer);
}

//-------------------------------------------------------------------------------------------------

void LicenseWidget::showScriptMissingErrorMsg()
{
    ui->scriptMissingErrorLabel->show();
}
