// Qt
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QFontMetrics>
#include <QPolygon>
#include <stdexcept>

// Application
#include "collapsiblepanel.h"

//-------------------------------------------------------------------------------------------------

CaptionLabel::CaptionLabel(const QString &caption, bool bExpandable, QWidget *parent)
    : QLabel(caption, parent), m_bCollpased(false), m_bExpandable(bExpandable)
{
    setStyleSheet("background-color: lightblue");
    setFixedHeight(48);
    setFrameStyle(QFrame::WinPanel);
    setFrameShadow(QFrame::Raised);
}

//-------------------------------------------------------------------------------------------------

CaptionLabel::~CaptionLabel()
{

}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
    {
        e->accept();
        m_bCollpased = !m_bCollpased;
        emit collapseOrExpand(m_bCollpased);
    }
    else e->ignore();
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::paintEvent(QPaintEvent *e)
{
    QLabel::paintEvent(e);

    if (m_bExpandable)
    {
        QPainter painter(this);
        QFontMetrics fm(this->font());
        int h = fm.height() - 4;
        h = h / 2 * 2; // h is even
        if (h > 0){
            int w = h / 2;
            int x = this->width() - 2 * h;
            int y = (this->height() - h) / 2;
            QPolygon tri(3);
            if (m_bCollpased){
                tri.setPoint(0, x, y);
                tri.setPoint(1, x + w + w, y);
                tri.setPoint(2, x + w, y + h);
            }
            else
            {
                tri.setPoint(0, x, y + h);
                tri.setPoint(1, x + w + w, y + h);
                tri.setPoint(2, x + w, y);
            }
            painter.setBrush(QBrush(QColor(Qt::black)));
            painter.drawPolygon(tri);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::collapse()
{
    m_bCollpased = true;
    emit collapseOrExpand(m_bCollpased);
}

//-------------------------------------------------------------------------------------------------

void CaptionLabel::expand()
{
    m_bCollpased = false;
    emit collapseOrExpand(m_bCollpased);
}

//-------------------------------------------------------------------------------------------------

CollapsiblePanel::CollapsiblePanel(const QString &caption, bool bHasParameters, QWidget *parent)
    : QWidget(parent), m_pWidget(nullptr), m_bHasParameters(bHasParameters)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLayout = new QVBoxLayout(this);
    m_pLabel = new CaptionLabel(caption, bHasParameters, this);
    m_pLayout->addWidget(m_pLabel);
    connect(m_pLabel, &CaptionLabel::collapseOrExpand, this, &CollapsiblePanel::collapseOrExpand);
}

//-------------------------------------------------------------------------------------------------

CollapsiblePanel::~CollapsiblePanel()
{

}

//-------------------------------------------------------------------------------------------------

QWidget *CollapsiblePanel::widget() const
{
    return m_pWidget;
}

//-------------------------------------------------------------------------------------------------

void CollapsiblePanel::setWidget(QWidget *widget)
{
    if (m_pWidget)
        throw std::runtime_error("CollapsiblePanel already has a widget");
    m_pWidget = widget;
    m_pWidget->setParent(this);
    m_pLayout->addWidget(m_pWidget);
    m_pLayout->setAlignment(m_pWidget, Qt::AlignTop);
}

//-------------------------------------------------------------------------------------------------

void CollapsiblePanel::setCaption(const QString &caption)
{
    m_pLabel->setText(caption);
}

//-------------------------------------------------------------------------------------------------

void CollapsiblePanel::collapseOrExpand(bool bCollapse)
{
    if (!m_pWidget || !m_bHasParameters)
        return;
    m_pWidget->setVisible(!bCollapse);
}

//-------------------------------------------------------------------------------------------------

bool CollapsiblePanel::isCollapsed() const
{
    return m_pLabel->isCollapsed();
}

//-------------------------------------------------------------------------------------------------

void CollapsiblePanel::collapse()
{
    collapseOrExpand(true);
}

//-------------------------------------------------------------------------------------------------

void CollapsiblePanel::expand()
{
    collapseOrExpand(false);
}

