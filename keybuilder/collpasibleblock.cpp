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

CollapsibleBlock::CollapsibleBlock(QWidget *pWidget, const QString &sCaption, bool bIsEmpty, QWidget *parent)
    : QWidget(parent), m_pWidget(nullptr), m_bIsEmpty(bIsEmpty), m_bIsCollapsed(true),
      m_bIsCurrent(false)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Create layout
    m_pLayout = new QVBoxLayout(this);

    // Create caption label
    m_pLabel = new CaptionLabel(this);
    m_pLabel->setCaption(sCaption);
    m_pLabel->setExpandable(!bIsEmpty);
    m_pLayout->addWidget(m_pLabel);

    // Do connections
    connect(m_pLabel, &CaptionLabel::blockSelected, this, &CollapsibleBlock::blockSelected);
    connect(this, &CollapsibleBlock::collapsedStateChanged, m_pLabel, &CaptionLabel::onStateChanged);
    connect(m_pLabel, &CaptionLabel::toggleCollapsedState, this, &CollapsibleBlock::onToggleCollapsedState);

    // Set widget
    setWidget(pWidget);
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
        m_pLayout->addWidget(m_pWidget);
        m_pLayout->setAlignment(m_pWidget, Qt::AlignTop);
        onCollapse(m_bIsCollapsed);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onCollapse(bool bCollapse)
{
    if (!m_pWidget || m_bIsEmpty)
        return;
    m_bIsCollapsed = bCollapse;
    m_pWidget->setVisible(!bCollapse);
    emit collapsedStateChanged(m_bIsCollapsed);
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

bool CollapsibleBlock::isCurrent() const
{
    return m_bIsCurrent;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::setCurrent(bool bCurrent)
{
    m_bIsCurrent = bCurrent;
    m_pLabel->setCurrent(bCurrent);
    m_pLabel->update();
}
