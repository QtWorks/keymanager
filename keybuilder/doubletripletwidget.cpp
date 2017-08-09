// Application
#include "doubletripletwidget.h"
#include "ui_doubletripletwidget.h"

//-------------------------------------------------------------------------------------------------

DoubleTripletWidget::DoubleTripletWidget(const QString &sLabel, const QString &sDefaultValue, QWidget *parent) : BaseWidget(parent),
    ui(new Ui::DoubleTripletWidget)
{
    ui->setupUi(this);
    m_sDefaultValue = sDefaultValue;
    ui->label->setText(sLabel);
    if (m_sDefaultValue.isEmpty())
        m_sDefaultValue = "[0,0,0]";

    QDoubleValidator *pValidator = new QDoubleValidator(0, 100, 3, this);
    ui->lineEdit1->setValidator(pValidator);
    ui->lineEdit2->setValidator(pValidator);
    ui->lineEdit3->setValidator(pValidator);

    m_sDefaultValue = "[0,0,0]";
    connect(ui->lineEdit1, &QLineEdit::textChanged, this, &DoubleTripletWidget::onTextChanged);
    connect(ui->lineEdit2, &QLineEdit::textChanged, this, &DoubleTripletWidget::onTextChanged);
    connect(ui->lineEdit3, &QLineEdit::textChanged, this, &DoubleTripletWidget::onTextChanged);
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
    QString sFormattedDefaultValue = m_sDefaultValue;
    sFormattedDefaultValue.replace("[", "");
    sFormattedDefaultValue.replace("]", "");
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

//-------------------------------------------------------------------------------------------------

void DoubleTripletWidget::onTextChanged()
{
    emit parameterValueChanged(m_sParameterVariable, value());
}
