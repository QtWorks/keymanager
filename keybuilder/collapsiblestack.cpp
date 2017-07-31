// Qt
#include <QVBoxLayout>

// Application
#include "collapsiblestack.h"
#include "collapsiblepanel.h"
#include "keyblock.h"

//-------------------------------------------------------------------------------------------------

CollapsibleStack::CollapsibleStack(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLayout = new QVBoxLayout(this);
    m_pLayout->setSpacing(0);
    setLayout(m_pLayout);
}

//-------------------------------------------------------------------------------------------------

CollapsibleStack::~CollapsibleStack()
{

}

//-------------------------------------------------------------------------------------------------

const QVector<CollapsiblePanel *> &CollapsibleStack::panels() const
{
    return m_vPanels;
}

//-------------------------------------------------------------------------------------------------

CollapsiblePanel *CollapsibleStack::addPanel(const QString &sCaption, QWidget *pKeyBlock)
{
    CollapsiblePanel *pPanel = new CollapsiblePanel(sCaption, this);
    connect(pPanel, &CollapsiblePanel::panelSelected, this, &CollapsibleStack::onPanelSelected);
    pPanel->setWidget(pKeyBlock);
    m_pLayout->addWidget(pPanel);
    m_pLayout->setAlignment(pPanel, Qt::AlignTop);
    m_vPanels << pPanel;
    return pPanel;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::expandAll()
{
    foreach (CollapsiblePanel *pPanel, m_vPanels)
    {
        if (pPanel->widget() != nullptr)
            pPanel->onCollapse(false);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::collapseAll()
{
    foreach (CollapsiblePanel *pPanel, m_vPanels)
    {
        if (pPanel->widget() != nullptr)
            pPanel->onCollapse(true);
    }
}

//-------------------------------------------------------------------------------------------------

bool CollapsibleStack::allCollapsed() const
{
    int n = m_pLayout->count();
    for (int i = 0; i < n; ++i)
    {
        CollapsiblePanel *panel = dynamic_cast<CollapsiblePanel *>(m_pLayout->itemAt(i)->widget());
        if (panel && !panel->isCollapsed())
            return false;
    }
    return true;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::onPanelSelected()
{
    CollapsiblePanel *pSelectedPanel = dynamic_cast<CollapsiblePanel *>(sender());
    if (pSelectedPanel != nullptr)
        emit panelSelected(pSelectedPanel);
}
