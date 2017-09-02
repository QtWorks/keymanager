// Qt
#include <QFont>
#include <QLineEdit>
#include <QClipboard>
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
    connect(ui->copyToClipBoardButton, &QPushButton::clicked, this, &KeyWidget::onCopyValueToClipBoard);
    showClipBoardButton(false);
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
    int iDelta = 15;
    for (int i=0; i<nBlocks; i++)
    {
        QLineEdit *pBlock = new QLineEdit(this);
        pBlock->setContentsMargins(0, 0, 0, 0);
        ui->innerLayout->addWidget(pBlock);
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

//-------------------------------------------------------------------------------------------------

void KeyWidget::showClipBoardButton(bool bShow)
{
    ui->copyToClipBoardButton->setVisible(bShow);
}

//-------------------------------------------------------------------------------------------------

void KeyWidget::onCopyValueToClipBoard()
{
    QClipboard *pClipBoard = QApplication::clipboard();
    pClipBoard->setText(getValue());
}
