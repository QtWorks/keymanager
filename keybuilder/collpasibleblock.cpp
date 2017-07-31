// Qt
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QFontMetrics>
#include <QPolygon>
#include <stdexcept>

// Application
#include "collapsibleblock.h"
#include "captionlabel.h"

//-------------------------------------------------------------------------------------------------

CollapsibleBlock::CollapsibleBlock(const QString &sCaption, bool bHasParameters, QWidget *parent)
    : QWidget(parent), m_pWidget(nullptr), m_bHasParameters(bHasParameters), m_bIsCollapsed(true)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLayout = new QVBoxLayout(this);
    m_pLabel = new CaptionLabel(this);
    m_pLabel->setCaption(sCaption);
    m_pLabel->setExpandable(bHasParameters);
    m_pLayout->addWidget(m_pLabel);
    connect(m_pLabel, &CaptionLabel::blockSelected, this, &CollapsibleBlock::blockSelected);
    connect(this, &CollapsibleBlock::stateChanged, m_pLabel, &CaptionLabel::onStateChanged);
    connect(m_pLabel, &CaptionLabel::toggleCollapsedState, this, &CollapsibleBlock::onToggleCollapsedState);
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock::~CollapsibleBlock()
{

}

//-------------------------------------------------------------------------------------------------

QWidget *CollapsibleBlock::widget() const
{
    return m_pWidget;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::setWidget(QWidget *widget)
{
    if (m_pWidget == nullptr)
    {
        m_pWidget = widget;
        m_pWidget->setParent(this);
        addWidgetInLayout(m_pWidget);
        onCollapse(m_bIsCollapsed);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onCollapse(bool bCollapse)
{
    if (!m_pWidget || !m_bHasParameters)
        return;
    m_bIsCollapsed = bCollapse;
    m_pWidget->setVisible(!bCollapse);
    emit stateChanged(m_bIsCollapsed);
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onToggleCollapsedState()
{
    onCollapse(!m_bIsCollapsed);
}

//-------------------------------------------------------------------------------------------------

bool CollapsibleBlock::isCollapsed() const
{
    return m_bIsCollapsed;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::setCurrent(bool bCurrent)
{
    m_pLabel->setCurrent(bCurrent);
    m_pLabel->update();
}

void CollapsibleBlock::addWidgetInLayout(QWidget *pWidget)
{
    if (pWidget != nullptr)
    {
        m_pLayout->addWidget(pWidget);
        m_pLayout->setAlignment(pWidget, Qt::AlignTop);
    }
}
