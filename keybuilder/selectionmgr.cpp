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
    if ((pBlock != nullptr) && pBlock->isSelected())
    {
        pBlock->select(false);
        if (pBlock->parentBlock()->isExclusive())
            pBlock->fullReset();
        QVector<CollapsibleBlock *> vChilds = pBlock->childBlocks();
        foreach (CollapsibleBlock *pChildBlock, vChilds)
            unselectBlock(pChildBlock);
        emit blockStatusChanged(pBlock);
    }
}

//-------------------------------------------------------------------------------------------------

void SelectionMgr::selectBlock(CollapsibleBlock *pBlock)
{
    if ((pBlock != nullptr) && (!pBlock->isSelected()))
    {
        CollapsibleBlock *pParentBlock = pBlock->parentBlock();
        if (pParentBlock != nullptr)
        {
            bool bParentBlockIsExclusive = pParentBlock->isExclusive();
            if (bParentBlockIsExclusive)
            {
                foreach (CollapsibleBlock *pChildBlock, pParentBlock->childBlocks())
                {
                    if (pChildBlock == pBlock)
                    {
                        pChildBlock->select(true);
                    }
                    else
                    {
                        unselectBlock(pChildBlock);
                    }
                }
            }
            else
            {
                if (pBlock->isSelected())
                    unselectBlock(pBlock);
                else pBlock->select(true);
            }
            selectBlock(pParentBlock);
        }
        if (pBlock->isSelected())
            pBlock->processBlockVariable();
        else pBlock->fullReset();
        emit blockStatusChanged(pBlock);
    }
}

//-------------------------------------------------------------------------------------------------

void SelectionMgr::onBlockSelected()
{
    CollapsibleBlock *pSelectedBlock = dynamic_cast<CollapsibleBlock *>(sender());
    selectThisBlock(pSelectedBlock);
}

//-------------------------------------------------------------------------------------------------

void SelectionMgr::selectThisBlock(CollapsibleBlock *pBlock)
{
    if (pBlock != nullptr)
    {
        // Retrieve parent block
        CollapsibleBlock *pParentBlock = pBlock->parentBlock();
        if (pParentBlock != nullptr)
        {
            // Exclusive?
            bool bParentIsExclusive = pParentBlock->isExclusive();
            if (bParentIsExclusive)
                // When we select a block, make sure block parents are also selected
                selectBlock(pBlock);
            else
            {
                if (pBlock->isSelected())
                    unselectBlock(pBlock);
                else
                    selectBlock(pBlock);
            }
        }
    }
}
