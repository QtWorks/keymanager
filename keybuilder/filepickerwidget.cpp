// Qt
#include <QFileDialog>

// Application
#include "filepickerwidget.h"
#include "ui_filepickerwidget.h"

//-------------------------------------------------------------------------------------------------

FilePickerWidget::FilePickerWidget(const QString &sLabel, const QString &sFileExtension, const QString &sDefaultValue, QWidget *parent) : BaseWidget(parent),
    ui(new Ui::FilePickerWidget), m_sFileExtension(sFileExtension)
{
    ui->setupUi(this);
    m_sDefaultValue = sDefaultValue;
    ui->label->setText(sLabel);
    connect(ui->openButton, &QPushButton::clicked, this, &FilePickerWidget::onOpenClicked);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &FilePickerWidget::textChanged);
}

//-------------------------------------------------------------------------------------------------

FilePickerWidget::~FilePickerWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void FilePickerWidget::onOpenClicked()
{
    QString sFileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", m_sFileExtension);
    if (!sFileName.isEmpty())
        ui->lineEdit->setText(sFileName);
}

//-------------------------------------------------------------------------------------------------

void FilePickerWidget::setLabel(const QString &sLabel)
{
    ui->label->setText(sLabel);
}

//-------------------------------------------------------------------------------------------------

void FilePickerWidget::setExtension(const QString &sExtenion)
{
    m_sFileExtension = sExtenion;
}

//-------------------------------------------------------------------------------------------------

void FilePickerWidget::applyDefaultValue()
{
    ui->lineEdit->setText(m_sDefaultValue);
}

//-------------------------------------------------------------------------------------------------

void FilePickerWidget::onFilePickerTextChanged()
{
    emit parameterValueChanged(m_sParameterVariable, ui->lineEdit->text());
}
