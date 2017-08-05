// Qt
#include <QVBoxLayout>
#include <QDebug>

// Application
#include "collapsiblestack.h"
#include "collapsibleblock.h"
#include "parameterblock.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

CollapsibleStack::CollapsibleStack(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pLayout = new QVBoxLayout(this);
    m_pLayout->setSpacing(0);
    setLayout(m_pLayout);
}

//-------------------------------------------------------------------------------------------------

CollapsibleStack::~CollapsibleStack()
{
    //qDebug() << "*** DESTROY COLLAPSIBLE STACK ***";
}

//-------------------------------------------------------------------------------------------------

QList<CollapsibleBlock *> CollapsibleStack::blocks() const
{
    return findChildren<CollapsibleBlock *>(COLLAPSIBLEBLOCK_OBJECT_NAME);
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock *CollapsibleStack::addBlock(const QString &sCaption, QWidget *pWidget, bool bIsEmpty)
{
    CollapsibleBlock *pBlock = new CollapsibleBlock(pWidget, sCaption, bIsEmpty, this);
    pBlock->setParent(this);
    m_pLayout->addWidget(pBlock);
    m_pLayout->setAlignment(pBlock, Qt::AlignTop);
    return pBlock;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::openAll()
{
    foreach (CollapsibleBlock *pBlock, blocks())
    {
        if (pBlock->widget() != nullptr)
            pBlock->onClose(false);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleStack::closeAll()
{
    foreach (CollapsibleBlock *pBlock, blocks())
    {
        if (pBlock->widget() != nullptr)
            pBlock->onClose(true);
    }
}


