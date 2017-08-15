#include "headerobject.h"
#include "ui_headerobject.h"

//-------------------------------------------------------------------------------------------------

HeaderObject::HeaderObject(QWidget *parent) : QWidget(parent),
    ui(new Ui::HeaderObject), m_iColumnIndex(-1)
{
    ui->setupUi(this);
}

//-------------------------------------------------------------------------------------------------

HeaderObject::HeaderObject(int iColumnIndex, const QString &sLabel, QWidget *parent) : QWidget(parent),
    ui(new Ui::HeaderObject), m_iColumnIndex(iColumnIndex)
{
    ui->setupUi(this);
    ui->label->setText(sLabel);
    connect(ui->pushButton, &QPushButton::clicked, this, &HeaderObject::onButtonClicked);
}

//-------------------------------------------------------------------------------------------------

HeaderObject::~HeaderObject()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void HeaderObject::onButtonClicked()
{
    emit clearClicked(m_iColumnIndex);
}
