#include "keywidget.h"
#include "ui_keywidget.h"

//-------------------------------------------------------------------------------------------------

KeyWidget::KeyWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::KeyWidget)
{
    ui->setupUi(this);
}

//-------------------------------------------------------------------------------------------------

KeyWidget::~KeyWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void KeyWidget::setTitle(const QString &sTitle)
{
    ui->label->setText(sTitle);
}

//-------------------------------------------------------------------------------------------------

QString KeyWidget::getValue() const
{
    QString sValue = QString("%1%2%3%4").arg(ui->field1->text()).arg(ui->field2->text()).arg(ui->field3->text()).arg(ui->field4->text());
    return sValue;
}
