// Qt
#include <QFont>

// Application
#include "keywidget.h"
#include "ui_keywidget.h"

//-------------------------------------------------------------------------------------------------

KeyWidget::KeyWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::KeyWidget)
{
    ui->setupUi(this);
    applyFont();
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

void KeyWidget::setReadOnly(bool bReadOnly)
{
    QVector<QLineEdit *> vInputs;
    vInputs << ui->field1 << ui->field2 << ui->field3 << ui->field4;
    foreach (QLineEdit *pLineEdit, vInputs)
        pLineEdit->setReadOnly(bReadOnly);
}

//-------------------------------------------------------------------------------------------------

QString KeyWidget::getValue() const
{
    QString sValue = QString("%1%2%3%4").arg(ui->field1->text()).arg(ui->field2->text()).arg(ui->field3->text()).arg(ui->field4->text());
    return sValue;
}

//-------------------------------------------------------------------------------------------------

void KeyWidget::setValue(const QString &sValue)
{
    QVector<QLineEdit *> vInputs;
    vInputs << ui->field1 << ui->field2 << ui->field3 << ui->field4;
    for (int i=0; i<4; i++)
    {
        QString sSubString = sValue.mid(i*4, 4);
        vInputs[i]->setText(sSubString);
    }
}

//-------------------------------------------------------------------------------------------------

void KeyWidget::applyFont()
{
    QFont font("Segoe UI", 24);
    QFontMetrics fm(font);
    QString sText("0000");
    int iPixelsWidth = fm.width(sText);
    int iPixelHeight = fm.height();

    // Resize input widgets
    QVector<QLineEdit *> vInputs;
    vInputs << ui->field1 << ui->field2 << ui->field3 << ui->field4;
    int iDelta = 8;
    foreach (QLineEdit *pInput, vInputs)
    {
        pInput->setFont(font);
        pInput->setFixedSize(iPixelsWidth+iDelta, iPixelHeight);
    }
}
