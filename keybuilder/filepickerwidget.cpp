// Qt
#include <QFileDialog>

// Application
#include "filepickerwidget.h"
#include "ui_filepickerwidget.h"

//-------------------------------------------------------------------------------------------------

FilePickerWidget::FilePickerWidget(const QString &sLabel, const QString &sFileExtension, QWidget *parent) : QWidget(parent),
    ui(new Ui::FilePickerWidget), m_sFileExtension(sFileExtension)
{
    ui->setupUi(this);
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

QString FilePickerWidget::value() const
{
    return ui->lineEdit->text();
}
