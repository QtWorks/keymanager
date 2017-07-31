// Qt
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QFontMetrics>
#include <QPolygon>
#include <stdexcept>

// Application
#include "collapsiblepanel.h"
#include "captionlabel.h"

//-------------------------------------------------------------------------------------------------

CollapsiblePanel::CollapsiblePanel(const QString &sCaption, bool bHasParameters, QWidget *parent)
    : QWidget(parent), m_pWidget(nullptr), m_bHasParameters(bHasParameters), m_bIsCollapsed(true)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLayout = new QVBoxLayout(this);
    m_pLabel = new CaptionLabel(this);
    m_pLabel->setCaption(sCaption);
    m_pLabel->setExpandable(bHasParameters);
    m_pLayout->addWidget(m_pLabel);
    connect(m_pLabel, &CaptionLabel::panelSelected, this, &CollapsiblePanel::panelSelected);
    connect(this, &CollapsiblePanel::stateChanged, m_pLabel, &CaptionLabel::onStateChanged);
    connect(m_pLabel, &CaptionLabel::toggleCollapsedState, this, &CollapsiblePanel::onToggleCollapsedState);
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
    if (m_pWidget == nullptr)
    {
        m_pWidget = widget;
        m_pWidget->setParent(this);
        m_pLayout->addWidget(m_pWidget);
        m_pLayout->setAlignment(m_pWidget, Qt::AlignTop);
        onCollapse(m_bIsCollapsed);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsiblePanel::onCollapse(bool bCollapse)
{
    if (!m_pWidget || !m_bHasParameters)
        return;
    m_bIsCollapsed = bCollapse;
    m_pWidget->setVisible(!bCollapse);
    emit stateChanged(m_bIsCollapsed);
}

//-------------------------------------------------------------------------------------------------

void CollapsiblePanel::onToggleCollapsedState()
{
    onCollapse(!m_bIsCollapsed);
}

//-------------------------------------------------------------------------------------------------

bool CollapsiblePanel::isCollapsed() const
{
    return m_bIsCollapsed;
}

//-------------------------------------------------------------------------------------------------

void CollapsiblePanel::setCurrent(bool bCurrent)
{
    m_pLabel->setCurrent(bCurrent);
    m_pLabel->update();
}

