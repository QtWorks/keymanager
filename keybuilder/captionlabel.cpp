// Qt
#include <QPainter>
#include <QPaintEvent>

// Application
#include "captionlabel.h"
#include "ui_captionlabel.h"

//-------------------------------------------------------------------------------------------------

CaptionLabel::CaptionLabel(QWidget *parent) : QWidget(parent),
    ui(new Ui::CaptionLabel),  m_bIsEnabled(true), m_bExpandable(true),
    m_bIsCurrent(false)
{
    ui->setupUi(this);
    connect(ui->openCloseButton, &QPushButton::clicked, this, &CaptionLabel::toggleState);
    connect(ui->clearAllButton, &QPushButton::clicked, this, &CaptionLabel::clearAll);
}

//-------------------------------------------------------------------------------------------------

CaptionLabel::~CaptionLabel()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::toggleState()
{
    emit toggleClosedState();
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::setCurrent(bool bCurrent)
{
    m_bIsCurrent = bCurrent;
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
    update();
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    QColor paintColor("lightblue");
    if (!m_bIsEnabled)
        paintColor.setNamedColor("lightgray");
    else {
        if (m_bIsCurrent)
            paintColor.setNamedColor("lightgreen");
    }
    p.fillRect(e->rect(), paintColor);
    QWidget::paintEvent(e);
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    emit blockSelected();
}

void CaptionLabel::onStateChanged(bool bIsClosed)
{
    setButtonLabel(bIsClosed ? tr("OPEN") : tr("CLOSE"));
}
