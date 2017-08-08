// Qt
#include <QVBoxLayout>
#include <QDebug>

// Application
#include "collapsiblestack.h"
#include "collapsibleblock.h"
#include "parameterblock.h"
#include "constants.h"

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
    qDebug() << "INFORMATION: DESTROY COLLAPSIBLE STACK";
}

//-------------------------------------------------------------------------------------------------

QVector<CollapsibleBlock *> CollapsibleStack::childBlocks() const
{
    return m_vBlocks;
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock *CollapsibleStack::addBlock(const CXMLNode &xBlock)
{
    CollapsibleBlock *pBlock = new CollapsibleBlock(xBlock, m_pController, this);
    m_vBlocks << pBlock;
    m_pLayout->addWidget(pBlock);
    m_pLayout->setAlignment(pBlock, Qt::AlignTop);
    return pBlock;
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


