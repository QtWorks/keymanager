// Qt
#include "lineeditwidget.h"
#include "ui_lineeditwidget.h"

// Application
#include "parameter.h"
#include "parametermgr.h"

//-------------------------------------------------------------------------------------------------

LineEditWidget::LineEditWidget(QWidget *parent) : BaseWidget(parent),
    ui(new Ui::LineEditWidget), m_sAuto(""), m_pParameterMgr(nullptr)
{
    ui->setupUi(this);
    m_sDefaultValue = "0";
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &LineEditWidget::valueChanged);
}

//-------------------------------------------------------------------------------------------------

LineEditWidget::LineEditWidget(const QString &sLabel, const QString &sDefaultValue, const QString &sAuto, QWidget *parent) : BaseWidget(parent),
    ui(new Ui::LineEditWidget), m_sAuto(sAuto)
{
    ui->setupUi(this);
    m_sDefaultValue = sDefaultValue;
    if (m_sDefaultValue.isEmpty())
        m_sDefaultValue = "0";
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &LineEditWidget::valueChanged);
    ui->label->setText(sLabel);
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

//-------------------------------------------------------------------------------------------------

void LineEditWidget::setLabel(const QString &sLabel)
{
    ui->label->setText(sLabel);
}

//-------------------------------------------------------------------------------------------------

void LineEditWidget::applyDefaultValue()
{
    ui->lineEdit->setText(m_sDefaultValue);
}

//-------------------------------------------------------------------------------------------------

void LineEditWidget::setWatchedParameters(const QHash<QString, Parameter *> &hParameters)
{
    m_hWatchedParameters = hParameters;
    for (QHash<QString, Parameter *>::iterator it=m_hWatchedParameters.begin(); it!=m_hWatchedParameters.end(); ++it)
        connect(it.value(), &Parameter::parameterValueChanged, this, &LineEditWidget::onEvaluateAutoScript);
}

//-------------------------------------------------------------------------------------------------

void LineEditWidget::setParameterMgr(ParameterMgr *pParameterMgr)
{
    m_pParameterMgr = pParameterMgr;
}

//-------------------------------------------------------------------------------------------------

void LineEditWidget::onEvaluateAutoScript()
{
    bool bSuccess = true;
    double dValue = m_pParameterMgr->evaluateAutoScript(m_sAuto, bSuccess);
    if (bSuccess)
        ui->lineEdit->setText(QString::number(dValue));
}
