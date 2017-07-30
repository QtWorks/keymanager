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
    : QWidget(parent), m_pWidget(nullptr), m_bHasParameters(bHasParameters)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLayout = new QVBoxLayout(this);
    m_pLabel = new CaptionLabel(this);
    m_pLabel->setCaption(sCaption);
    m_pLabel->setExpandable(bHasParameters);
    connect(m_pLabel, &CaptionLabel::panelSelected, this, &CollapsiblePanel::panelSelected);
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

void CollapsiblePanel::collapseOrExpand(bool bCollapse)
{
    if (!m_pWidget || !m_bHasParameters)
        return;
    m_pWidget->setVisible(!bCollapse);
    m_pLabel->setButtonLabel(bCollapse ? tr("OPEN") : tr("CLOSE"));
}

//-------------------------------------------------------------------------------------------------

bool CollapsiblePanel::isCollapsed() const
{
    return m_pLabel->isCollapsed();
}

//-------------------------------------------------------------------------------------------------

void CollapsiblePanel::setCurrent(bool bCurrent)
{
    m_pLabel->setCurrent(bCurrent);
    m_pLabel->update();
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

