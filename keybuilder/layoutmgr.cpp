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
#include "collapsiblepanel.h"
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

CollapsiblePanel *LayoutMgr::addBlock(QWidget *pBlock, const QString &sName)
{
    CollapsiblePanel *pAddedPanel = nullptr;
    if (pBlock != nullptr)
    {
        CollapsibleStack *pTargetStack = nullptr;
        int iStackIndex = m_nBlocks/MAX_KEY_PER_STACK;
        if (iStackIndex < m_vStacks.size())
            pTargetStack = m_vStacks[iStackIndex];
        else
        {
            pTargetStack = new CollapsibleStack(this);
            connect(pTargetStack, &CollapsibleStack::panelSelected, this, &LayoutMgr::onPanelSelected);
            m_vStacks << pTargetStack;
            ui->horizontalLayout->addWidget(pTargetStack);
            ui->horizontalLayout->setAlignment(pTargetStack, Qt::AlignTop);
        }
        pAddedPanel = pTargetStack->addPanel(sName, pBlock);
        m_nBlocks++;
    }
    return pAddedPanel;
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

void LayoutMgr::onPanelSelected(CollapsiblePanel *pCurrentPanel)
{
    foreach (CollapsibleStack *pStack, m_vStacks)
    {
        QVector<CollapsiblePanel *> vPanels = pStack->panels();
        foreach (CollapsiblePanel *pPanel, vPanels)
            pPanel->setCurrent(pPanel == pCurrentPanel);
    }
}
