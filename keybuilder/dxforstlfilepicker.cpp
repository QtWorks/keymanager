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
}

//-------------------------------------------------------------------------------------------------

DXForSTLFilePicker::~DXForSTLFilePicker()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::onSelectSTL()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open File"), Utils::appDir().absolutePath(), "*.stl");
    if (!sFileName.isEmpty())
    {
        ui->stlLineEdit->setText(sFileName);
        ui->dxfLineEdit->clear();
        emit parameterValueChanged(m_sSTLVariable, sFileName);
    }
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::onSelectDXF()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open File"), Utils::appDir().absolutePath(), "*.dxf");
    if (!sFileName.isEmpty())
    {
        ui->dxfLineEdit->setText(sFileName);
        ui->stlLineEdit->clear();
        emit parameterValueChanged(m_sDXFVariable, sFileName);
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
