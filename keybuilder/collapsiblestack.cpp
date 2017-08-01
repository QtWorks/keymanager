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

CollapsibleBlock *CollapsibleStack::addBlock(const QString &sCaption, QWidget *pWidget, bool bIsEmpty)
{
    CollapsibleBlock *pBlock = new CollapsibleBlock(pWidget, sCaption, bIsEmpty, this);
    connect(pBlock, &CollapsibleBlock::blockSelected, this, &CollapsibleStack::onBlockSelected);
    pBlock->setParent(this);
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

void CollapsibleStack::onBlockSelected()
{
    CollapsibleBlock *pSelectedBlock = dynamic_cast<CollapsibleBlock *>(sender());
    if (pSelectedBlock != nullptr)
        emit blockSelected(pSelectedBlock);
}
