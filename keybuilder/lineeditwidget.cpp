// Qt
#include "lineeditwidget.h"
#include "ui_lineeditwidget.h"

// Application
#include "controller.h"
#include "parameter.h"
#include "parametermgr.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

LineEditWidget::LineEditWidget(Controller *pController, const QString &sLabel, const QString &sDefaultValue, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent) :
    BaseWidget(pController, parent), ui(new Ui::LineEditWidget)
{
    ui->setupUi(this);
    m_sDefaultValue = sDefaultValue;
    if (m_sDefaultValue.isEmpty())
        m_sDefaultValue = PROPERTY_DEFAULT_VALUE;
    m_sAutoScript = sAutoScript;
    m_sEnabledCondition = sEnabledCondition;
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &LineEditWidget::onTextChanged, Qt::UniqueConnection);
    ui->label->setText(sLabel);
}

//-------------------------------------------------------------------------------------------------

LineEditWidget::~LineEditWidget()
{
    delete ui;
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

void LineEditWidget::onTextChanged()
{
    emit parameterValueChanged(m_sParameterVariable, ui->lineEdit->text());
}

//-------------------------------------------------------------------------------------------------

void LineEditWidget::onEvaluateAutoScript()
{
    bool bSuccess = true;
    QString sValue = m_pController->parameterMgr()->evaluateAutoScript(m_sAutoScript, bSuccess);
    if (bSuccess)
        ui->lineEdit->setText(sValue);
}

