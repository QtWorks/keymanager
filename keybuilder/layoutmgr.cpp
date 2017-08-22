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
#include "blockmodel.h"
#include "selectionmgr.h"
#define NSTACKS 2

//-------------------------------------------------------------------------------------------------

LayoutMgr::LayoutMgr(QWidget *parent) : QWidget(parent), ui(new Ui::LayoutMgr),
    m_nBlocks(0), m_iSize(0), m_nCols(0), m_nBlockPerStack(0), m_pController(nullptr),
    m_pRootCollapsibleBlock(nullptr)
{
    ui->setupUi(this);

    // Build selection mgr
    m_pSelectionMgr = new SelectionMgr(this);

    // Build block model
    m_pBlockModel = new BlockModel(this);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setModel(m_pBlockModel);
    connect(m_pBlockModel, &BlockModel::highlightItem, this, &LayoutMgr::onHighlightItem, Qt::UniqueConnection);

    // Listen to block status changed from selection mgr
    connect(m_pSelectionMgr, &SelectionMgr::blockSelectionStatusChanged, m_pBlockModel, &BlockModel::onBlockSelectionStatusChanged, Qt::UniqueConnection);
    connect(m_pSelectionMgr, &SelectionMgr::blockSelectionStatusChanged, this, &LayoutMgr::onBlockSelectionStatusChanged, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

LayoutMgr::~LayoutMgr()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::buildMenu(const CXMLNode &xBlock)
{
    // Process blocks
    m_pRootCollapsibleBlock = new CollapsibleBlock(xBlock, m_pController, this);
    m_pRootCollapsibleBlock->setVisible(false);

    // Retrieve childs
    QVector<CollapsibleBlock *> vRootChildBlocks = m_pRootCollapsibleBlock->childBlocks();
    setSize(vRootChildBlocks.size());
    foreach (CollapsibleBlock *pChildBlock, vRootChildBlocks)
    {
        // Add child block to stack
        if (pChildBlock != nullptr)
            addBlockToStack(pChildBlock);
    }
    connectBlocksToSelectionMgr(m_pRootCollapsibleBlock);
    evaluateEnabledCondition(m_pRootCollapsibleBlock);
    m_pBlockModel->setRootBlock(m_pRootCollapsibleBlock);
    ui->treeView->collapseAll();
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
    m_pSelectionMgr->setController(m_pController);
    ui->keyPreviewWidget->setController(m_pController);
    connect(m_pController, &Controller::updateKeyPreviews, ui->keyPreviewWidget, &KeyPreviewWidget::onUpdateKeyImage);
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::addBlockToStack(CollapsibleBlock *pBlock)
{
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
            pTargetStack = new CollapsibleStack(m_pController, this);
            m_vStacks << pTargetStack;
            ui->horizontalLayout->addWidget(pTargetStack);
            ui->horizontalLayout->setAlignment(pTargetStack, Qt::AlignTop);
        }
        if (pTargetStack != nullptr)
        {
            pTargetStack->addBlock(pBlock);
            m_nBlocks++;
        }
    }
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::connectBlocksToSelectionMgr(CollapsibleBlock *pBlock)
{
    if (pBlock != nullptr)
    {
        connect(pBlock, &CollapsibleBlock::selectMe, m_pSelectionMgr, &SelectionMgr::onSelectBlock, Qt::UniqueConnection);
        connect(pBlock, &CollapsibleBlock::blockSelectionStatusChanged, m_pSelectionMgr, &SelectionMgr::blockSelectionStatusChanged);
        connect(pBlock, &CollapsibleBlock::closedStateChanged, this, &LayoutMgr::onClosedStateChanged, Qt::UniqueConnection);
        foreach (CollapsibleBlock *pChildBlock, pBlock->childBlocks())
            connectBlocksToSelectionMgr(pChildBlock);
    }
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::evaluateEnabledCondition(CollapsibleBlock *pBlock)
{
    if (pBlock != nullptr)
    {
        if (pBlock->parameterBlock() != nullptr)
            pBlock->parameterBlock()->onEvaluateEnabledCondition();
        foreach (CollapsibleBlock *pChildBlock, pBlock->childBlocks())
            evaluateEnabledCondition(pChildBlock);
    }
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
    ui->treeView->expandAll();
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onCloseAll()
{
    foreach (CollapsibleStack *pStack, m_vStacks)
        pStack->closeAll();
    ui->treeView->collapseAll();
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onClearAll()
{
    m_pRootCollapsibleBlock->onClearAll();
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onHighlightItem(const QModelIndex &index, CollapsibleBlock *pBlock)
{
    if (index.isValid() && (pBlock != nullptr))
    {
        m_pBlockModel->updateIndex(index);
    }
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onBlockSelectionStatusChanged(CollapsibleBlock *pBlock)
{
    if (pBlock != nullptr)
    {
        if (pBlock->isSelected())
        {
            ParameterBlock *pParameterBlock = pBlock->parameterBlock();
            if (pParameterBlock != nullptr)
            {
                if (!pParameterBlock->keyPreviewImage().isEmpty())
                    m_pController->loadKeyPreviewImage(pParameterBlock->keyPreviewImage());
            }
        }
        pBlock->setBlockVariable();
    }
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onClosedStateChanged(bool bClosed)
{
    CollapsibleBlock *pSender = dynamic_cast<CollapsibleBlock *>(sender());
    if (pSender != nullptr)
    {
        QModelIndex blockIndex = m_pBlockModel->getBlockIndex(pSender->uid());
        if (blockIndex.isValid())
        {
            if (bClosed)
                ui->treeView->collapse(blockIndex);
            else
                ui->treeView->expand(blockIndex);
        }
    }
}
