// Qt
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QDoubleValidator>

// Application
#include "layoutmgr.h"
#include "ui_layoutmgr.h"
#include "keyblock.h"
#include "collapsiblestack.h"
#include "collapsibleblock.h"
#include "filepicker.h"
#define MAX_KEY_PER_STACK 5

//-------------------------------------------------------------------------------------------------

LayoutMgr::LayoutMgr(QWidget *parent) : QWidget(parent), ui(new Ui::LayoutMgr),
    m_nBlocks(0)
{
    ui->setupUi(this);
}

//-------------------------------------------------------------------------------------------------

LayoutMgr::~LayoutMgr()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock *LayoutMgr::addBlock(QWidget *pBlock, const QString &sName, bool bHasParameters)
{
    CollapsibleBlock *pAddedBlock = nullptr;
    if (pBlock != nullptr)
    {
        CollapsibleStack *pTargetStack = nullptr;
        int iStackIndex = m_nBlocks/MAX_KEY_PER_STACK;
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
        pAddedBlock = pTargetStack->setWidget(sName, pBlock, bHasParameters);
        m_nBlocks++;
    }
    return pAddedBlock;
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onExpandAll()
{
    foreach (CollapsibleStack *pStack, m_vStacks)
        pStack->collapseAll();
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onCollapseAll()
{
    foreach (CollapsibleStack *pStack, m_vStacks)
        pStack->expandAll();
}

//-------------------------------------------------------------------------------------------------

void LayoutMgr::onBlockSelected(CollapsibleBlock *pCurrentBlock)
{
    foreach (CollapsibleStack *pStack, m_vStacks)
    {
        QVector<CollapsibleBlock *> vBlocks = pStack->blocks();
        foreach (CollapsibleBlock *pBlock, vBlocks)
            pBlock->setCurrent(pBlock == pCurrentBlock);
    }
}
