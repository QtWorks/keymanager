// Qt
#include <QVBoxLayout>

// Application
#include "collapsiblestack.h"
#include "collapsibleblock.h"
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

const QVector<CollapsibleBlock *> &CollapsibleStack::blocks() const
{
    return m_vBlocks;
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock *CollapsibleStack::setWidget(const QString &sCaption, QWidget *pWidget, bool bHasParameters)
{
    CollapsibleBlock *pBlock = new CollapsibleBlock(sCaption, bHasParameters, this);
    connect(pBlock, &CollapsibleBlock::blockSelected, this, &CollapsibleStack::onBlockSelected);
    pBlock->setWidget(pWidget);
    m_pLayout->addWidget(pBlock);
    m_pLayout->setAlignment(pBlock, Qt::AlignTop);
    m_vBlocks << pBlock;
    return pBlock;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::expandAll()
{
    foreach (CollapsibleBlock *pBlock, m_vBlocks)
    {
        if (pBlock->widget() != nullptr)
            pBlock->onCollapse(false);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::collapseAll()
{
    foreach (CollapsibleBlock *pBlock, m_vBlocks)
    {
        if (pBlock->widget() != nullptr)
            pBlock->onCollapse(true);
    }
}

//-------------------------------------------------------------------------------------------------

bool CollapsibleStack::allCollapsed() const
{
    int n = m_pLayout->count();
    for (int i = 0; i < n; ++i)
    {
        CollapsibleBlock *pBlock = dynamic_cast<CollapsibleBlock *>(m_pLayout->itemAt(i)->widget());
        if (pBlock && !pBlock->isCollapsed())
            return false;
    }
    return true;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::onBlockSelected()
{
    CollapsibleBlock *pSelectedBlock = dynamic_cast<CollapsibleBlock *>(sender());
    if (pSelectedBlock != nullptr)
        emit blockSelected(pSelectedBlock);
}
