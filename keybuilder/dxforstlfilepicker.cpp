// Qt
#include <QFileDialog>

// Application
#include "dxforstlfilepicker.h"
#include "ui_dxforstlfilepicker.h"
#include "constants.h"
#include "utils.h"

//-------------------------------------------------------------------------------------------------

DXForSTLFilePicker::DXForSTLFilePicker(Controller *pController, const QString &sDefaultValue, const QString &sSTLVariable, const QString &sDXFVariable, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent) :
    BaseWidget(pController, parent), ui(new Ui::DXForSTLFilePicker), m_sSTLVariable(sSTLVariable), m_sDXFVariable(sDXFVariable)
{
    ui->setupUi(this);
    m_hWidgetHash[m_sSTLVariable] = ui->stlLineEdit;
    m_hWidgetHash[m_sDXFVariable] = ui->dxfLineEdit;

    // Set default value
    setDefaultValue(sDefaultValue);

    // Set auto script
    setAutoScript(sAutoScript);

    // Set enabled condition
    setEnabledCondition(sEnabledCondition);

    connect(ui->selectDXFButton, &QPushButton::clicked, this, &DXForSTLFilePicker::onSelectDXF, Qt::UniqueConnection);
    connect(ui->selectSTLButton, &QPushButton::clicked, this, &DXForSTLFilePicker::onSelectSTL, Qt::UniqueConnection);
    connect(ui->dxfLineEdit, &QLineEdit::textChanged, this, &DXForSTLFilePicker::onDXFTextChanged, Qt::UniqueConnection);
    connect(ui->stlLineEdit, &QLineEdit::textChanged, this, &DXForSTLFilePicker::onSTLTextChanged, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

DXForSTLFilePicker::~DXForSTLFilePicker()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::onSelectSTL()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open File"), Utils::settingsValue(LAST_VISITED_DIR), "*.stl");
    if (!sFileName.isEmpty())
    {
        Utils::updateSettings(sFileName);
        ui->stlLineEdit->setText(sFileName);
        ui->dxfLineEdit->clear();
    }
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::onSelectDXF()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open File"), Utils::settingsValue(LAST_VISITED_DIR), "*.dxf");
    if (!sFileName.isEmpty())
    {
        Utils::updateSettings(sFileName);
        ui->dxfLineEdit->setText(sFileName);
        ui->stlLineEdit->clear();
    }
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::setValue(const QString &sVariableName, const QString &sVariableValue)
{
    QLineEdit *pLineEdit = m_hWidgetHash[sVariableName];
    if (pLineEdit != nullptr)
        pLineEdit->setText(sVariableValue);
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::applyValue(const QString &sValue)
{
    QStringList lSplitted = sValue.split(",");
    if (lSplitted.size() == 2)
    {
        ui->dxfLineEdit->setText(lSplitted.first());
        ui->stlLineEdit->setText(lSplitted[1]);
    }
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::applyDefaultValue()
{
    applyValue(defaultValue());
}

//-------------------------------------------------------------------------------------------------

const QString &DXForSTLFilePicker::stlVariable() const
{
    return m_sDXFVariable;
}

//-------------------------------------------------------------------------------------------------

const QString &DXForSTLFilePicker::dxfVariable() const
{
    return m_sSTLVariable;
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::onDXFTextChanged()
{
    emit parameterValueChanged(m_sDXFVariable, ui->dxfLineEdit->text());
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::onSTLTextChanged()
{
    emit parameterValueChanged(m_sSTLVariable, ui->stlLineEdit->text());
}

