// Qt
#include <QVBoxLayout>

// Application
#include "collapsiblestack.h"
#include "collapsiblepanel.h"

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

CollapsiblePanel *CollapsibleStack::addPanel(const QString &sCaption, QWidget *pWidget)
{
    CollapsiblePanel *pPanel = new CollapsiblePanel(sCaption, (pWidget != nullptr), this);
    if (pWidget != nullptr)
        pPanel->setWidget(pWidget);
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
            pPanel->expand();
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::collapseAll()
{
    foreach (CollapsiblePanel *pPanel, m_vPanels)
    {
        if (pPanel->widget() != nullptr)
            pPanel->collapse();
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
