// Qt
#include <QFileDialog>

// Application
#include "dxforstlfilepicker.h"
#include "ui_dxforstlfilepicker.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

DXForSTLFilePicker::DXForSTLFilePicker(const QString &sDefaultValue, const QString &sSTLVariable, const QString &sDXFVariable, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent) : BaseWidget(parent),
    ui(new Ui::DXForSTLFilePicker), m_sSTLVariable(sSTLVariable), m_sDXFVariable(sDXFVariable)
{
    ui->setupUi(this);
    m_sDefaultValue = sDefaultValue;
    if (m_sDefaultValue.isEmpty())
        m_sDefaultValue = PROPERTY_DEFAULT_VALUE;
    m_sAutoScript = sAutoScript;
    m_sEnabledCondition = sEnabledCondition;
    connect(ui->selectDXFButton, &QPushButton::clicked, this, &DXForSTLFilePicker::onSelectDXF);
    connect(ui->selectSTLButton, &QPushButton::clicked, this, &DXForSTLFilePicker::onSelectSTL);
}

//-------------------------------------------------------------------------------------------------

DXForSTLFilePicker::~DXForSTLFilePicker()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::onSelectSTL()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "*.stl");
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
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "*.dxf");
    if (!sFileName.isEmpty())
    {
        ui->dxfLineEdit->setText(sFileName);
        ui->stlLineEdit->clear();
        emit parameterValueChanged(m_sDXFVariable, sFileName);
    }
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::applyDefaultValue()
{
    ui->dxfLineEdit->setText(m_sDefaultValue);
    ui->stlLineEdit->setText(m_sDefaultValue);
}
