// Qt
#include <QFont>
#include <QLineEdit>
#include <QDebug>

// Application
#include "keywidget.h"
#include "ui_keywidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

KeyWidget::KeyWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::KeyWidget)
{
    ui->setupUi(this);
}

//-------------------------------------------------------------------------------------------------

KeyWidget::~KeyWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void KeyWidget::setTitle(const QString &sTitle)
{
    ui->label->setText(sTitle);
}

//-------------------------------------------------------------------------------------------------

void KeyWidget::setReadOnly(bool bReadOnly)
{
    foreach (QLineEdit *pBlock, m_vBlocks)
        pBlock->setReadOnly(bReadOnly);
}

//-------------------------------------------------------------------------------------------------

QString KeyWidget::getValue() const
{
    QString sValue("");
    for (int i=0; i<m_vBlocks.size(); i++)
        sValue += m_vBlocks[i]->text();
    return sValue;
}

//-------------------------------------------------------------------------------------------------

void KeyWidget::setValue(const QString &sValue)
{
    int iLength = sValue.length();
    int nBlocks = (iLength/4) + (iLength%4 != 0 ? 1 : 0);
    qDebug() << sValue << nBlocks;
    int iDelta = 12;
    for (int i=0; i<nBlocks; i++)
    {
        qDebug() << i << nBlocks;
        QLineEdit *pBlock = new QLineEdit(this);
        pBlock->setContentsMargins(0, 0, 0, 0);
        ui->blockLayout->addWidget(pBlock);
        pBlock->setText(sValue.mid(i*4, 4));
        QFont font("Segoe UI", 24);
        QFontMetrics fm(font);
        QString sText("0000");
        int iPixelsWidth = fm.width(sText);
        int iPixelHeight = fm.height();
        pBlock->setFont(font);
        pBlock->setFixedSize(iPixelsWidth+iDelta, iPixelHeight);
        m_vBlocks << pBlock;
    }
}

