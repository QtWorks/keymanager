// Qt
#include <QFont>
#include <QLineEdit>
#include <QClipboard>
#include <QKeyEvent>
#include <QDebug>

// Application
#include "keywidget.h"
#include "ui_keywidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

KeyWidget::KeyWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::KeyWidget), m_iBlockCount(0)
{
    // Setup UI
    ui->setupUi(this);

    // Copy to clipboard
    ui->copyToClipBoardButton->hide();
    connect(ui->copyToClipBoardButton, &QPushButton::clicked, this, &KeyWidget::onCopyValueToClipBoard);
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
    if (sValue != getValue())
    {
        // Compute number of blocks
        int iLength = sValue.length();
        int nBlocks = (iLength/4) + (iLength%4 != 0 ? 1 : 0);
        if (nBlocks > m_iBlockCount)
            nBlocks = m_iBlockCount;
        for (int i=0; i<m_iBlockCount; i++)
            m_vBlocks[i]->setText(i < nBlocks ? sValue.mid(i*4, 4): "");
    }
}

//-------------------------------------------------------------------------------------------------

void KeyWidget::showClipBoardButton(bool bShow)
{
    ui->copyToClipBoardButton->setVisible(bShow);
}

//-------------------------------------------------------------------------------------------------

void KeyWidget::setBlockCount(int iBlockCount)
{
    m_iBlockCount = iBlockCount;
    buildBlocks();
}

//-------------------------------------------------------------------------------------------------

void KeyWidget::onCopyValueToClipBoard()
{
    QClipboard *pClipBoard = QApplication::clipboard();
    pClipBoard->setText(getValue());
}

//-------------------------------------------------------------------------------------------------

bool KeyWidget::eventFilter(QObject *pWatched, QEvent *event)
{
    QLineEdit *pLineEdit = dynamic_cast<QLineEdit *>(pWatched);
    if (pLineEdit != nullptr)
    {
        QKeyEvent *pKeyEvent = dynamic_cast<QKeyEvent *>(event);
        if (pKeyEvent != nullptr)
        {
            if ((pKeyEvent->key() == Qt::Key_V) && (pKeyEvent->modifiers() == Qt::ControlModifier))
            {
                event->ignore();
                QClipboard *pClipBoard = QApplication::clipboard();
                setValue(pClipBoard->text());
                return true;
            }
        }
    }
    return QWidget::eventFilter(pWatched, event);
}

//-------------------------------------------------------------------------------------------------

void KeyWidget::buildBlocks()
{
    int iDelta = 15;
    for (int i=0; i<m_iBlockCount; i++)
    {
        QLineEdit *pBlock = new QLineEdit(this);
        pBlock->setMaxLength(4);
        pBlock->installEventFilter(this);
        pBlock->setContextMenuPolicy(Qt::NoContextMenu);
        pBlock->setContentsMargins(0, 0, 0, 0);
        ui->innerLayout->addWidget(pBlock);
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
