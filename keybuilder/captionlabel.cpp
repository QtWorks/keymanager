// Qt
#include <QPainter>
#include <QPaintEvent>

// Application
#include "captionlabel.h"
#include "ui_captionlabel.h"
#include "collapsibleblock.h"

//-------------------------------------------------------------------------------------------------

CaptionLabel::CaptionLabel(QWidget *parent) : QWidget(parent),
    ui(new Ui::CaptionLabel),  m_bIsEnabled(true), m_bExpandable(true),
    m_bIsCurrent(false)
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
    repaint();
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
    ui->openCloseButton->setVisible(bExpandable);
    ui->clearAllButton->setVisible(bExpandable);
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::updateEnabledState(bool bEnabled)
{
    m_bIsEnabled = bEnabled;
    ui->openCloseButton->setVisible(bEnabled);
    ui->clearAllButton->setVisible(bEnabled);
    repaint();
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
            paintColor.setNamedColor("lightgreen");
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
    emit selectMe();
    emit openOrClose();
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::onClearAll()
{
    emit clearAll();
}
