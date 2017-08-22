// Qt
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

// Application
#include "captionlabel.h"
#include "ui_captionlabel.h"
#include "collapsibleblock.h"

//-------------------------------------------------------------------------------------------------

CaptionLabel::CaptionLabel(QWidget *parent) : QWidget(parent),
    ui(new Ui::CaptionLabel),  m_bIsEnabled(true), m_bExpandable(true),
    m_bIsCurrent(false), m_bBlockAlwaysOpened(false), m_bCanClearBlock(true)
{
    ui->setupUi(this);
    connect(ui->openCloseButton, &QPushButton::clicked, this, &CaptionLabel::onOpenOrClose, Qt::UniqueConnection);
    connect(ui->clearAllButton, &QPushButton::clicked, this, &CaptionLabel::onClearAll, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

CaptionLabel::~CaptionLabel()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::setCurrent(bool bCurrent)
{
    m_bIsCurrent = bCurrent;
    update();
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::setCaption(const QString &sCaption)
{
    ui->label->setText(sCaption);
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::setButtonLabel(const QString &sButtonLabel)
{
    ui->openCloseButton->setText(sButtonLabel);
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::setExpandable(bool bExpandable)
{
    m_bExpandable = bExpandable;
    ui->openCloseButton->setVisible(!m_bBlockAlwaysOpened && bExpandable);
    ui->clearAllButton->setVisible(m_bCanClearBlock && bExpandable);
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::updateEnabledState(bool bEnabled)
{
    m_bIsEnabled = bEnabled;
    ui->openCloseButton->setVisible(!m_bBlockAlwaysOpened && m_bExpandable && bEnabled);
    ui->clearAllButton->setVisible(m_bCanClearBlock && m_bExpandable && bEnabled);
    update();
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::setBlockAlwaysOpened(bool bBlockAlwaysOpened)
{
    m_bBlockAlwaysOpened = bBlockAlwaysOpened;
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::setCanClearBlock(bool bCanClearBlock)
{
    m_bCanClearBlock = bCanClearBlock;
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    QPainter p(this);
    QColor paintColor("#EEEEEE");
    if (m_bIsEnabled)
    {
        if (m_bIsCurrent)
            paintColor.setNamedColor("orange");
        else
            paintColor.setNamedColor("#EEEEEE");
    }
    else
    {
        paintColor.setNamedColor("#B1B1B1");
    }
    p.fillRect(e->rect(), paintColor);
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    emit selectMe();
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::onStateChanged(bool bIsClosed)
{
    setButtonLabel(bIsClosed ? tr("OPEN") : tr("CLOSE"));
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::onOpenOrClose()
{
    emit openOrClose();
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::onClearAll()
{
    emit clearAll();
}
