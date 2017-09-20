// Qt
#include "lineeditwidget.h"
#include "ui_lineeditwidget.h"

// Application
#include "controller.h"
#include "parameter.h"
#include "parametermgr.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

LineEditWidget::LineEditWidget(Controller *pController, const QString &sLabel, const QString &sDefaultValue, const QString &sAutoScript,
    const QString &sEnabledCondition, bool bValidateOnReturnPressed, QWidget *parent) :
    BaseWidget(pController, parent), ui(new Ui::LineEditWidget)
{
    ui->setupUi(this);

    // Set default value
    setDefaultValue(sDefaultValue);

    // Set auto script
    setAutoScript(sAutoScript);

    // Set enabled condition
    setEnabledCondition(sEnabledCondition);

    if (bValidateOnReturnPressed)
        connect(ui->lineEdit, &QLineEdit::returnPressed, this, &LineEditWidget::onTextChanged, Qt::UniqueConnection);
    else
        connect(ui->lineEdit, &QLineEdit::textChanged, this, &LineEditWidget::onTextChanged, Qt::UniqueConnection);

    setLabel(sLabel);
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
    QFont font;
    font.setFamily("Segoe UI");
    font.setPixelSize(16);
    QFontMetrics fm(font);

    // Find max text width
    int iMaxTextWidth = fm.width(sLabel);
    ui->label->setFixedWidth(qMax(172, iMaxTextWidth));
    ui->label->setText(sLabel);
}

//-------------------------------------------------------------------------------------------------

void LineEditWidget::applyDefaultValue()
{
    ui->lineEdit->setText(defaultValue());
    onTextChanged();
}

//-------------------------------------------------------------------------------------------------

void LineEditWidget::applyValue(const QString &sValue)
{
    ui->lineEdit->setText(sValue);
    onTextChanged();
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

