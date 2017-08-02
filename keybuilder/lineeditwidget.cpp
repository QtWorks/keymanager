#include "lineeditwidget.h"
#include "ui_lineeditwidget.h"

//-------------------------------------------------------------------------------------------------

LineEditWidget::LineEditWidget(const QString &sLabel, QWidget *parent) : QWidget(parent),
    ui(new Ui::LineEditWidget)
{
    ui->setupUi(this);
    ui->label->setText(sLabel);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &LineEditWidget::valueChanged);
}

//-------------------------------------------------------------------------------------------------

LineEditWidget::~LineEditWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

QString LineEditWidget::value() const
{
    return ui->lineEdit->text();
}

//-------------------------------------------------------------------------------------------------

void LineEditWidget::setValidator(QValidator *pValidator)
{
    ui->lineEdit->setValidator(pValidator);
}
