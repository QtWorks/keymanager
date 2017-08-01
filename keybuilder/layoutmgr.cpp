// Qt
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QDoubleValidator>

// Application
#include "layoutmgr.h"
#include "ui_layoutmgr.h"
#include "parameterblock.h"
#include "collapsiblestack.h"
#include "collapsibleblock.h"
#include "filepicker.h"
#include "constants.h"
#define NSTACKS 3

//-------------------------------------------------------------------------------------------------

LayoutMgr::LayoutMgr(QWidget *parent) : QWidget(parent), ui(new Ui::LayoutMgr),
    m_nBlocks(0), m_iSize(0), m_nCols(0), m_nBlockPerStack(0)
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
    QVector<CXMLNode> vBlocks = xNode.getNodesByTagName(TAG_BLOCK);
    setSize(vBlocks.size());
    foreach (CXMLNode xParameterBlock, vBlocks)
        addCollapsibleBlockToStack(xParameterBlock);
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::addCollapsibleBlockToStack(const CXMLNode &xBlock)
{
    // Create new parameter block
    ParameterBlock *pParameterBlock = new ParameterBlock(xBlock, this);

    // Listen to parameter value changed
    connect(pParameterBlock, &ParameterBlock::parameterValueChanged, this, &LayoutMgr::parameterValueChanged);
    CollapsibleBlock *pNewBlock = addBlock(pParameterBlock, pParameterBlock->name(), pParameterBlock->isEmpty());
    connect(pNewBlock, &CollapsibleBlock::blockSelected, pParameterBlock, &ParameterBlock::onSelectMe);
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
            connect(pTargetStack, &CollapsibleStack::blockSelected, this, &LayoutMgr::onBlockSelected);
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

void LayoutMgr::onBlockSelected(CollapsibleBlock *pCurrentBlock)
{
    foreach (CollapsibleStack *pStack, m_vStacks)
    {
        QList<CollapsibleBlock *> lBlocks = pStack->blocks();
        foreach (CollapsibleBlock *pBlock, lBlocks)
            pBlock->setCurrent(pBlock == pCurrentBlock);
    }
}
