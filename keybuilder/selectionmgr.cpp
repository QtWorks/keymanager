// Qt
#include <QDebug>

// Application
#include "selectionmgr.h"
#include "collapsibleblock.h"
#include "parameterblock.h"

//-------------------------------------------------------------------------------------------------

SelectionMgr::SelectionMgr(QObject *parent) : QObject(parent),
    m_pController(nullptr)
{

}

//-------------------------------------------------------------------------------------------------

SelectionMgr::~SelectionMgr()
{

}

//-------------------------------------------------------------------------------------------------

void SelectionMgr::setController(Controller *pController)
{
    m_pController = pController;
}

//-------------------------------------------------------------------------------------------------

void SelectionMgr::unselectBlock(CollapsibleBlock *pBlock)
{
    if (pBlock != nullptr)
    {
        pBlock->select(false);
        foreach (CollapsibleBlock *pChildBlock, pBlock->childBlocks())
            if (pChildBlock != nullptr)
                unselectBlock(pChildBlock);
    }
}

//-------------------------------------------------------------------------------------------------

void SelectionMgr::selectBlock(CollapsibleBlock *pBlock)
{
    if (pBlock != nullptr)
    {
        pBlock->select(true);
        if (pBlock->parentBlock() != nullptr)
            selectBlock(pBlock->parentBlock());
    }
}

//-------------------------------------------------------------------------------------------------

void SelectionMgr::onSelectBlock()
{
    CollapsibleBlock *pSelectedBlock = dynamic_cast<CollapsibleBlock *>(sender());
    if ((pSelectedBlock != nullptr) && (pSelectedBlock->isEnabled()))
    {
        // Retrieve parent block
        CollapsibleBlock *pParentBlock = pSelectedBlock->parentBlock();
        if (pParentBlock != nullptr)
        {
            // Exclusive?
            bool bParentIsExclusive = pParentBlock->isExclusive();
            if (bParentIsExclusive)
            {
                foreach (CollapsibleBlock *pChildBlock, pParentBlock->childBlocks())
                {
                    if (pSelectedBlock == pChildBlock)
                        selectBlock(pChildBlock);
                    else
                        unselectBlock(pChildBlock);
                }
            }
            else
            {
                if (pSelectedBlock->isSelected())
                    unselectBlock(pSelectedBlock);
                else
                    selectBlock(pSelectedBlock);
            }
        }
    }
}
