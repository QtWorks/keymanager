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
#define NSTACKS 2

//-------------------------------------------------------------------------------------------------

LayoutMgr::LayoutMgr(QWidget *parent) : QWidget(parent), ui(new Ui::LayoutMgr),
    m_nBlocks(0), m_iSize(0), m_nCols(0), m_nBlockPerStack(0), m_pController(nullptr),
    m_pRootCollapsibleBlock(nullptr)
{
    ui->setupUi(this);
    m_pBlockModel = new BlockModel(this);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setModel(m_pBlockModel);
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

    m_pBlockModel->setRootBlock(m_pRootCollapsibleBlock);
    ui->treeView->expandAll();
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

void LayoutMgr::onClearAll()
{
    m_pRootCollapsibleBlock->onClearAll();
}

