// Qt
#include <QPainter>
#include <QPaintEvent>

// Application
#include "captionlabel.h"
#include "ui_captionlabel.h"

//-------------------------------------------------------------------------------------------------

CaptionLabel::CaptionLabel(QWidget *parent) : QWidget(parent),
    ui(new Ui::CaptionLabel), m_bExpandable(true),
    m_bIsCurrent(false)
{
    ui->setupUi(this);
    connect(ui->openCloseButton, &QPushButton::clicked, this, &CaptionLabel::toggleState);
}

//-------------------------------------------------------------------------------------------------

CaptionLabel::~CaptionLabel()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::toggleState()
{
    emit toggleCollapsedState();
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
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    QColor paintColor("lightblue");
    if (m_bIsCurrent)
        paintColor.setNamedColor("lightgreen");
    p.fillRect(e->rect(), paintColor);
    QWidget::paintEvent(e);
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    emit blockSelected();
}

void CaptionLabel::onStateChanged(bool bIsCollapsed)
{
    setButtonLabel(bIsCollapsed ? tr("OPEN") : tr("CLOSE"));
}
