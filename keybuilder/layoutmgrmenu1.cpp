// Application
#include "layoutmgrmenu1.h"
#include "collapsiblestack.h"
#include "collapsibleblock.h"

//-------------------------------------------------------------------------------------------------

LayoutMgrMenu1::LayoutMgrMenu1(QWidget *parent) : LayoutMgr(parent)
{

}

//-------------------------------------------------------------------------------------------------

LayoutMgrMenu1::~LayoutMgrMenu1()
{

}

//-------------------------------------------------------------------------------------------------

void LayoutMgrMenu1::buildMenu(const CXMLNode &xNode)
{
    LayoutMgr::buildMenu(xNode);
    foreach (CollapsibleBlock *pBlock, allBlocks())
        connect(pBlock, &CollapsibleBlock::blockSelected, this, &LayoutMgrMenu1::onBlockSelected);
}

//-------------------------------------------------------------------------------------------------

void LayoutMgrMenu1::onBlockSelected()
{
    CollapsibleBlock *pSender = dynamic_cast<CollapsibleBlock *>(sender());
    if (pSender != nullptr)
    {
        foreach (CollapsibleStack *pStack, m_vStacks)
        {
            QList<CollapsibleBlock *> lBlocks = pStack->blocks();
            foreach (CollapsibleBlock *pBlock, lBlocks)
                pBlock->setCurrent(pBlock == pSender);
        }
    }
}
