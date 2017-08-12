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
    setDefaultValue(sDefaultValue);
    if (defaultValue().isEmpty())
        setDefaultValue(PROPERTY_DEFAULT_VALUE);
    setAutoScript(sAutoScript);
    setEnabledCondition(sEnabledCondition);
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
    ui->lineEdit->setText(defaultValue());
}

//-------------------------------------------------------------------------------------------------

void LineEditWidget::onTextChanged()
{
    emit parameterValueChanged(parameterVariable(), ui->lineEdit->text());
}

//-------------------------------------------------------------------------------------------------

void LineEditWidget::onEvaluateAutoScript()
{
    bool bSuccess = true;
    QString sValue = m_pController->parameterMgr()->evaluateAutoScript(autoScript(), bSuccess);
    if (bSuccess)
        ui->lineEdit->setText(sValue);
}

