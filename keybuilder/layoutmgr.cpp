// Qt
#include <QDebug>

// Application
#include "controller.h"
#include "layoutmgr.h"
#include "ui_layoutmgr.h"
#include "parameterblock.h"
#include "collapsiblestack.h"
#include "collapsibleblock.h"
#include "filepickerwidget.h"
#include "parametermgr.h"
#include "constants.h"
#define NSTACKS 3

//-------------------------------------------------------------------------------------------------

LayoutMgr::LayoutMgr(QWidget *parent) : QWidget(parent), ui(new Ui::LayoutMgr),
    m_nBlocks(0), m_iSize(0), m_nCols(0), m_nBlockPerStack(0), m_pController(nullptr),
    m_pRootBlock(nullptr)
{
    ui->setupUi(this);
}

//-------------------------------------------------------------------------------------------------

LayoutMgr::~LayoutMgr()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::buildMenu(const CXMLNode &xNode)
{
    // Create root parameter block
    m_pRootBlock = new ParameterBlock(xNode, m_pController, false);
    QVector<CXMLNode> vBlocks = xNode.getNodesByTagName(TAG_BLOCK);
    setSize(vBlocks.size());
    foreach (CXMLNode xParameterBlock, vBlocks)
        addCollapsibleBlockToStack(xParameterBlock, m_pRootBlock);
    foreach (CollapsibleBlock *pBlock, topLevelBlocks())
        connect(pBlock, &CollapsibleBlock::blockSelected, this, &LayoutMgr::onBlockSelected);
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::addCollapsibleBlockToStack(const CXMLNode &xBlock, ParameterBlock *pParentParameterBlock)
{
    // Create new parameter block
    ParameterBlock *pParameterBlock = new ParameterBlock(xBlock, m_pController);
    pParameterBlock->setParentBlock(pParentParameterBlock);

    // This is a root block, set exclusive state
    bool bIsExclusive = true;
    QString sExclusive = xBlock.attributes()[PROPERTY_EXCLUSIVE].simplified();
    if (!sExclusive.isEmpty())
        bIsExclusive = (sExclusive == VALUE_TRUE);
    pParameterBlock->setExclusive(bIsExclusive);

    // Listen to parameter value changed
    addBlock(pParameterBlock, pParameterBlock->name(), pParameterBlock->isEmpty());
}

//-------------------------------------------------------------------------------------------------

Controller *LayoutMgr::controller() const
{
    return m_pController;
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::setController(Controller *pController)
{
    m_pController = pController;
}

//-------------------------------------------------------------------------------------------------

QList<CollapsibleBlock *> LayoutMgr::topLevelBlocks() const
{
    QList<CollapsibleBlock *> lBlocks;
    foreach (CollapsibleStack *pStack, m_vStacks)
        lBlocks << pStack->blocks();
    return lBlocks;
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock *LayoutMgr::addBlock(QWidget *pBlock, const QString &sName, bool bIsEmpty)
{
    CollapsibleBlock *pAddedBlock = nullptr;
    if (pBlock != nullptr)
    {
        CollapsibleStack *pTargetStack = nullptr;
        int iStackIndex = m_nBlocks/m_nBlockPerStack;
        if (iStackIndex > (NSTACKS-1))
            iStackIndex = NSTACKS-1;
        if (iStackIndex < m_vStacks.size())
            pTargetStack = m_vStacks[iStackIndex];
        else
        {
            pTargetStack = new CollapsibleStack(this);
            m_vStacks << pTargetStack;
            ui->horizontalLayout->addWidget(pTargetStack);
            ui->horizontalLayout->setAlignment(pTargetStack, Qt::AlignTop);
        }
        pAddedBlock = pTargetStack->addBlock(sName, pBlock, bIsEmpty);
        m_nBlocks++;
    }
    return pAddedBlock;
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::setSize(int iSize)
{
    if (iSize > 0)
    {
        m_iSize = iSize;
        m_nCols = (m_iSize < NSTACKS) ? 1 : NSTACKS;
        m_nBlockPerStack = qRound((double)m_iSize/(double)m_nCols);
    }
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onOpenAll()
{
    foreach (CollapsibleStack *pStack, m_vStacks)
        pStack->openAll();
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onCloseAll()
{
    foreach (CollapsibleStack *pStack, m_vStacks)
        pStack->closeAll();
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onBlockSelected()
{
    CollapsibleBlock *pSender = dynamic_cast<CollapsibleBlock *>(sender());
    if (pSender != nullptr)
    {
        // Get parameter block
        ParameterBlock *pParameterBlock = dynamic_cast<ParameterBlock *>(pSender->widget());
        if (pParameterBlock != nullptr)
        {
            // Process block variable
            if (!pParameterBlock->variable().isEmpty())
                m_pController->parameterMgr()->setParameterValue(pParameterBlock->variable(), pParameterBlock->value());

            // Retrieve parent
            ParameterBlock *pParentParameterBlock = pParameterBlock->parentBlock();
            if (pParentParameterBlock != nullptr)
            {
                // Exclusive?
                bool bParentIsExclusive = pParentParameterBlock->isExclusive();

                // Get owner collapsible block
                CollapsibleBlock *pOwnerCollapsibleBlock = dynamic_cast<CollapsibleBlock *>(pParentParameterBlock->parentWidget());
                QList<CollapsibleBlock *> vChildBlocks = (pOwnerCollapsibleBlock == nullptr) ? topLevelBlocks() : pOwnerCollapsibleBlock->childBlocks();
                foreach (CollapsibleBlock *pBlock, vChildBlocks)
                {
                    if (bParentIsExclusive)
                        pBlock->setCurrent(pBlock == pSender);
                    else
                    {
                        if (pBlock == pSender)
                            pBlock->setCurrent(!pBlock->isCurrent());
                    }
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onClearAll()
{
    m_pRootBlock->clearAll();
    foreach (CollapsibleBlock *pBlock, topLevelBlocks())
        pBlock->onClearAll();
}
