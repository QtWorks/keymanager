#include "xyzmirrorwidget.h"
#include "ui_xyzmirrorwidget.h"

//-------------------------------------------------------------------------------------------------

XYZMirrorWidget::XYZMirrorWidget(Controller *pController, const QString &sDefaultValue, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent) :
    BaseWidget(pController, parent), ui(new Ui::XYZMirrorWidget)
{
    ui->setupUi(this);

    // Set default value
    setDefaultValue(sDefaultValue);

    // Set auto script
    setAutoScript(sAutoScript);

    // Set enabled condition
    setEnabledCondition(sEnabledCondition);

    connect(ui->xCheckBox, &QCheckBox::clicked, this, &XYZMirrorWidget::onXCheckedChanged, Qt::UniqueConnection);
    connect(ui->yCheckBox, &QCheckBox::clicked, this, &XYZMirrorWidget::onYCheckedChanged, Qt::UniqueConnection);
    connect(ui->zCheckBox, &QCheckBox::clicked, this, &XYZMirrorWidget::onZCheckedChanged, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

XYZMirrorWidget::~XYZMirrorWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void XYZMirrorWidget::applyDefaultValue()
{
    applyValue(defaultValue());
}

//-------------------------------------------------------------------------------------------------

void XYZMirrorWidget::applyValue(const QString &sValue)
{
    ui->xCheckBox->blockSignals(true);
    ui->yCheckBox->blockSignals(true);
    ui->zCheckBox->blockSignals(true);

    QString sFormattedDefaultValue = sValue;
    sFormattedDefaultValue.replace("[", "");
    sFormattedDefaultValue.replace("]", "");
    QStringList lSplitted = sFormattedDefaultValue.split(",");
    if (lSplitted.size() != 3)
    {
        lSplitted.clear();
        lSplitted << "0" << "0" << "0";
    }

    ui->xCheckBox->setChecked(lSplitted.first().simplified() != "0");
    ui->yCheckBox->setChecked(lSplitted[1].simplified() != "0");
    ui->zCheckBox->setChecked(lSplitted[2].simplified() != "0");

    ui->xCheckBox->blockSignals(false);
    ui->yCheckBox->blockSignals(false);
    ui->zCheckBox->blockSignals(false);
    emit parameterValueChanged(parameterVariable(), value());
}

//-------------------------------------------------------------------------------------------------

QString XYZMirrorWidget::value() const
{
    QString sFormattedValue = QString("[%1,%2,%3]").arg(ui->xCheckBox->isChecked() ? "1" : "0").
            arg(ui->yCheckBox->isChecked() ? "1" : "0").arg(ui->zCheckBox->isChecked() ? "1" : "0");
    return sFormattedValue;
}

//-------------------------------------------------------------------------------------------------

void XYZMirrorWidget::onXCheckedChanged(bool bChecked)
{
    Q_UNUSED(bChecked);
    emit parameterValueChanged(parameterVariable(), value());
}

//-------------------------------------------------------------------------------------------------

void XYZMirrorWidget::onYCheckedChanged(bool bChecked)
{
    Q_UNUSED(bChecked);
    emit parameterValueChanged(parameterVariable(), value());
}

//-------------------------------------------------------------------------------------------------

void XYZMirrorWidget::onZCheckedChanged(bool bChecked)
{
    Q_UNUSED(bChecked);
    emit parameterValueChanged(parameterVariable(), value());
}
