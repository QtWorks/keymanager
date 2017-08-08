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
    m_pRootCollapsibleBlock(nullptr)
{
    ui->setupUi(this);
}

//-------------------------------------------------------------------------------------------------

LayoutMgr::~LayoutMgr()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::buildMenu(const CXMLNode &xBlock)
{
    // Create root parameter block and root collapsible block (invisible)
    m_pRootCollapsibleBlock = new CollapsibleBlock(xBlock, m_pController, this);
    m_pRootCollapsibleBlock->setVisible(false);

    QVector<CXMLNode> vBlocks = xBlock.getNodesByTagName(TAG_BLOCK);
    setSize(vBlocks.size());
    foreach (CXMLNode xParameterBlock, vBlocks)
    {
        // Add block
        CollapsibleBlock *pAddedBlock = addBlockToStack(xParameterBlock);

        // Add child recursively
        if (pAddedBlock != nullptr)
            m_pRootCollapsibleBlock->addChildBlock(pAddedBlock);
    }
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

CollapsibleBlock *LayoutMgr::addBlockToStack(const CXMLNode &xBlock)
{
    CollapsibleStack *pTargetStack = nullptr;
    CollapsibleBlock *pAddedBlock = nullptr;
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
        pAddedBlock = pTargetStack->addBlock(xBlock);
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

void LayoutMgr::onClearAll()
{
    m_pRootCollapsibleBlock->onClearAll();
}

