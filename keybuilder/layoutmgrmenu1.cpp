// Application
#include "layoutmgrmenu1.h"
#include "collapsiblestack.h"
#include "collapsibleblock.h"
#include "controller.h"
#include "parametermgr.h"
#include "constants.h"
#include "parameterblock.h"

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
            {
                pBlock->setCurrent(pBlock == pSender);
                if (pBlock == pSender)
                {
                    ParameterBlock *pParameterBlock = dynamic_cast<ParameterBlock *>(pSender->widget());
                    if (pParameterBlock != nullptr)
                    {
                        if (pParameterBlock->variable() == PARAMETER_TYPE_OF_KEY)
                            controller()->parameterMgr()->setParameterValue(PARAMETER_TYPE_OF_KEY, pParameterBlock->value());
                    }
                }
            }
        }
    }
}
