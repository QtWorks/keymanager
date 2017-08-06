// Qt
#include <QFileDialog>

// Application
#include "dxforstlfilepicker.h"
#include "ui_dxforstlfilepicker.h"

//-------------------------------------------------------------------------------------------------

DXForSTLFilePicker::DXForSTLFilePicker(QWidget *parent) : BaseWidget(parent),
    ui(new Ui::DXForSTLFilePicker)
{
    ui->setupUi(this);
    m_sDefaultValue = "";
    connect(ui->selectDXFButton, &QPushButton::clicked, this, &DXForSTLFilePicker::onSelectDXF);
    connect(ui->selectSTLButton, &QPushButton::clicked, this, &DXForSTLFilePicker::onSelectSTL);
    connect(ui->dxfLineEdit, &QLineEdit::textChanged, this, &DXForSTLFilePicker::dxfSelected);
    connect(ui->stlLineEdit, &QLineEdit::textChanged, this, &DXForSTLFilePicker::stlSelected);
}

//-------------------------------------------------------------------------------------------------

DXForSTLFilePicker::DXForSTLFilePicker(const QString &sDefaultValue, QWidget *parent) : BaseWidget(parent),
    ui(new Ui::DXForSTLFilePicker)
{
    ui->setupUi(this);
    m_sDefaultValue = sDefaultValue;
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
    }
}

//-------------------------------------------------------------------------------------------------

void DXForSTLFilePicker::onSelectDXF()
{
    QString sFileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "*.stl");
    if (!sFileName.isEmpty())
    {
        ui->dxfLineEdit->setText(sFileName);
        ui->stlLineEdit->clear();
    }
}

//-------------------------------------------------------------------------------------------------

QString DXForSTLFilePicker::dxfValue() const
{
    return ui->dxfLineEdit->text();
}

QString DXForSTLFilePicker::stlValue() const
{
    return ui->stlLineEdit->text();
}
