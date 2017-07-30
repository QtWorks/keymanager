// Application
#include "keyblock.h"
#include "ui_keyblock.h"

//-------------------------------------------------------------------------------------------------

KeyBlock::KeyBlock(QWidget *parent) : QWidget(parent), ui(new Ui::KeyBlock)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

//-------------------------------------------------------------------------------------------------

KeyBlock::~KeyBlock()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void KeyBlock::addWidget(QWidget *pWidget)
{
    ui->verticalLayout->addWidget(pWidget);
    ui->verticalLayout->setAlignment(pWidget, Qt::AlignTop);
}

//-------------------------------------------------------------------------------------------------

const QString &KeyBlock::name() const
{
    return m_sName;
}

//-------------------------------------------------------------------------------------------------

void KeyBlock::setName(const QString &sName)
{
    m_sName = sName;
}

