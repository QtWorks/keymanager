// Application
#include "doubletripletwidget.h"
#include "ui_doubletripletwidget.h"

//-------------------------------------------------------------------------------------------------

DoubleTripletWidget::DoubleTripletWidget(const QString &sLabel, QWidget *parent) : QWidget(parent),
    ui(new Ui::DoubleTripletWidget)
{
    ui->setupUi(this);
    ui->label->setText(sLabel);
    connect(ui->lineEdit1, &QLineEdit::textChanged, this, &DoubleTripletWidget::valueChanged);
    connect(ui->lineEdit2, &QLineEdit::textChanged, this, &DoubleTripletWidget::valueChanged);
    connect(ui->lineEdit3, &QLineEdit::textChanged, this, &DoubleTripletWidget::valueChanged);
}

//-------------------------------------------------------------------------------------------------

DoubleTripletWidget::~DoubleTripletWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

QString DoubleTripletWidget::value() const
{
    QString sFormattedValue = QString("[%1,%2,%3]").arg(ui->lineEdit1->text()).arg(ui->lineEdit2->text()).arg(ui->lineEdit3->text());
    return sFormattedValue;
}
