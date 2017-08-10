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
        pBlock->fullReset();
        QVector<CollapsibleBlock *> vChilds = pBlock->childBlocks();
        foreach (CollapsibleBlock *pChildBlock, vChilds)
            unselectBlock(pChildBlock);
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
            ParameterBlock *pParentParameterBlock = pParentBlock->parameterBlock();
            if (pParentParameterBlock != nullptr)
            {
                bool bParentBlockIsExclusive = pParentParameterBlock->isExclusive();
                if (bParentBlockIsExclusive)
                {
                    foreach (CollapsibleBlock *pChildBlock, pParentBlock->childBlocks())
                        pChildBlock->select(pChildBlock == pBlock);
                }
                else
                {
                    if (pBlock->isSelected())
                        unselectBlock(pBlock);
                    else pBlock->select(true);
                }
            }
            selectBlock(pParentBlock);
        }
        if (pBlock->isSelected())
            pBlock->processBlockVariable();
        else pBlock->fullReset();
    }
}

//-------------------------------------------------------------------------------------------------

void SelectionMgr::onBlockSelected()
{
    CollapsibleBlock *pSelectedBlock = dynamic_cast<CollapsibleBlock *>(sender());
    if (pSelectedBlock != nullptr)
    {
        // Retrieve parent block
        CollapsibleBlock *pParentBlock = pSelectedBlock->parentBlock();
        if (pParentBlock != nullptr)
        {
            // Retrieve parent parameter block
            ParameterBlock *pParentParameterBlock = pParentBlock->parameterBlock();
            if (pParentParameterBlock != nullptr)
            {
                // Exclusive?
                bool bParentIsExclusive = pParentParameterBlock->isExclusive();
                if (bParentIsExclusive)
                    // When we select a block, make sure block parents are also selected
                    selectBlock(pSelectedBlock);
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
}
