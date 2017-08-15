// Qt
#include <QVBoxLayout>
#include <QDebug>

// Application
#include "collapsiblestack.h"
#include "collapsibleblock.h"
#include "parameterblock.h"
#include "constants.h"
#include "helper.h"

//-------------------------------------------------------------------------------------------------

CollapsibleStack::CollapsibleStack(Controller *pController, QWidget *parent) : QWidget(parent),
    m_pController(pController)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLayout = new QVBoxLayout(this);
    m_pLayout->setSpacing(0);
    setLayout(m_pLayout);
}

//-------------------------------------------------------------------------------------------------

CollapsibleStack::~CollapsibleStack()
{
    logMessage("INFORMATION: DESTROY COLLAPSIBLE STACK");
}

//-------------------------------------------------------------------------------------------------

QVector<CollapsibleBlock *> CollapsibleStack::childBlocks() const
{
    return m_vBlocks;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::addBlock(CollapsibleBlock *pBlock)
{
    if (pBlock != nullptr)
    {
        m_vBlocks << pBlock;
        m_pLayout->addWidget(pBlock);
        m_pLayout->setAlignment(pBlock, Qt::AlignTop);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::openAll()
{
    foreach (CollapsibleBlock *pBlock, m_vBlocks)
        if (pBlock != nullptr)
            pBlock->onClose(false);
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::closeAll()
{
    foreach (CollapsibleBlock *pBlock, m_vBlocks)
        if (pBlock != nullptr)
            pBlock->onClose(true);
}


