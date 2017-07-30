// Qt
#include <QFileDialog>

// Application
#include "filepicker.h"
#include "ui_filepicker.h"

//-------------------------------------------------------------------------------------------------

FilePicker::FilePicker(const QString &sFileExtension, QWidget *parent) : QWidget(parent),
    ui(new Ui::FilePicker), m_sFileExtension(sFileExtension)
{
    ui->setupUi(this);
    connect(ui->openButton, &QPushButton::clicked, this, &FilePicker::onOpenClicked);
}

//-------------------------------------------------------------------------------------------------

FilePicker::~FilePicker()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void FilePicker::onOpenClicked()
{
    QString sFileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", m_sFileExtension);
    if (!sFileName.isEmpty())
        ui->lineEdit->setText(sFileName);
}

//-------------------------------------------------------------------------------------------------

QLineEdit *FilePicker::fileLineEdit() const
{
    return ui->lineEdit;
}
