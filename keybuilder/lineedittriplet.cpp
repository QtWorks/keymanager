// Application
#include "lineedittriplet.h"
#include "ui_lineedittriplet.h"

//-------------------------------------------------------------------------------------------------

LineEditTriplet::LineEditTriplet(QWidget *parent) : QWidget(parent),
    ui(new Ui::LineEditTriplet)
{
    ui->setupUi(this);
    connect(ui->lineEdit1, &QLineEdit::textChanged, this, &LineEditTriplet::valueChanged);
    connect(ui->lineEdit2, &QLineEdit::textChanged, this, &LineEditTriplet::valueChanged);
    connect(ui->lineEdit3, &QLineEdit::textChanged, this, &LineEditTriplet::valueChanged);
}

//-------------------------------------------------------------------------------------------------

LineEditTriplet::~LineEditTriplet()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

QString LineEditTriplet::value() const
{
    QString sFormattedValue = QString("[%1,%2,%3]").arg(ui->lineEdit1->text()).arg(ui->lineEdit2->text()).arg(ui->lineEdit3->text());
    return sFormattedValue;
}
