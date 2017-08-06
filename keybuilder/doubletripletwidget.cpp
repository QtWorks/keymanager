// Application
#include "doubletripletwidget.h"
#include "ui_doubletripletwidget.h"

//-------------------------------------------------------------------------------------------------

DoubleTripletWidget::DoubleTripletWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::DoubleTripletWidget), m_sDefaultValue("")
{
    ui->setupUi(this);
    connect(ui->lineEdit1, &QLineEdit::textChanged, this, &DoubleTripletWidget::valueChanged);
    connect(ui->lineEdit2, &QLineEdit::textChanged, this, &DoubleTripletWidget::valueChanged);
    connect(ui->lineEdit3, &QLineEdit::textChanged, this, &DoubleTripletWidget::valueChanged);
}

//-------------------------------------------------------------------------------------------------

DoubleTripletWidget::DoubleTripletWidget(const QString &sLabel, const QString &sDefaultValue, QWidget *parent) : QWidget(parent),
    ui(new Ui::DoubleTripletWidget), m_sDefaultValue(sDefaultValue)
{
    ui->setupUi(this);
    ui->label->setText(sLabel);
    if (m_sDefaultValue.isEmpty())
        m_sDefaultValue = "[0,0,0]";
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

void DoubleTripletWidget::setLabel(const QString &sLabel)
{
    ui->label->setText(sLabel);
}

//-------------------------------------------------------------------------------------------------

QString DoubleTripletWidget::value() const
{
    QString sFormattedValue = QString("[%1,%2,%3]").arg(ui->lineEdit1->text()).arg(ui->lineEdit2->text()).arg(ui->lineEdit3->text());
    return sFormattedValue;
}

//-------------------------------------------------------------------------------------------------

void DoubleTripletWidget::applyDefaultValue()
{
    if (!m_sDefaultValue.isEmpty())
    {
        QString sFormattedDefaultValue = m_sDefaultValue;
        sFormattedDefaultValue = sFormattedDefaultValue.replace("[", "");
        sFormattedDefaultValue = sFormattedDefaultValue.replace("]", "");
        QStringList lSplitted = sFormattedDefaultValue.split(",");
        if (lSplitted.size() != 3)
        {
            lSplitted.clear();
            lSplitted << "0" << "0" << "0";
        }
        bool bOK = true;
        lSplitted.first().toDouble(&bOK);
        if (bOK)
        {
            lSplitted[1].toDouble(&bOK);
            if (bOK) {
                lSplitted[2].toDouble(&bOK);
                if (bOK)
                {
                    ui->lineEdit1->setText(lSplitted.first());
                    ui->lineEdit2->setText(lSplitted[1]);
                    ui->lineEdit3->setText(lSplitted[2]);
                }
                else
                {
                    ui->lineEdit1->setText("0");
                    ui->lineEdit2->setText("0");
                    ui->lineEdit3->setText("0");
                }
            }
        }
    }
}
