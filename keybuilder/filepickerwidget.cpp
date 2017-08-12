// Qt
#include <QFileDialog>

// Application
#include "filepickerwidget.h"
#include "ui_filepickerwidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

FilePickerWidget::FilePickerWidget(Controller *pController, const QString &sLabel, const QString &sFileExtension, const QString &sDefaultValue, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent) :
    BaseWidget(pController, parent), ui(new Ui::FilePickerWidget), m_sFileExtension(sFileExtension)
{
    ui->setupUi(this);
    setDefaultValue(sDefaultValue);
    if (defaultValue().isEmpty())
        setDefaultValue(PROPERTY_DEFAULT_VALUE);
    setAutoScript(sAutoScript);
    setEnabledCondition(sEnabledCondition);
    ui->label->setText(sLabel);
    connect(ui->openButton, &QPushButton::clicked, this, &FilePickerWidget::onOpenClicked, Qt::UniqueConnection);
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &FilePickerWidget::textChanged, Qt::UniqueConnection);
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
    applyValue(defaultValue());
}

void FilePickerWidget::applyValue(const QString &sValue)
{
    ui->lineEdit->setText(sValue);
}

//-------------------------------------------------------------------------------------------------

void FilePickerWidget::onFilePickerTextChanged()
{
    emit parameterValueChanged(parameterVariable(), ui->lineEdit->text());
}
