// Qt
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QUuid>
#include <QDebug>

// Application
#include "collapsibleblock.h"
#include "captionlabel.h"
#include "constants.h"
#include "parameterblock.h"
#include "controller.h"
#include "parametermgr.h"
#include "selectionmgr.h"
#include "helper.h"

//-------------------------------------------------------------------------------------------------

CollapsibleBlock::CollapsibleBlock(const CXMLNode &xBlock, Controller *pController, QWidget *parent) : QWidget(parent),
    m_pParameterBlock(nullptr), m_bIsClosed(true), m_bIsSelected(false), m_pParentBlock(nullptr), m_pController(pController),
    m_sUID(QUuid::createUuid().toString())
{
    // Set size policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Create layout
    m_pLayout = new QVBoxLayout(this);

    // Create caption label
    m_pCaptionLabel = new CaptionLabel(this);
    m_pLayout->addWidget(m_pCaptionLabel);
    connect(m_pCaptionLabel, &CaptionLabel::selectMe, this, &CollapsibleBlock::selectMe, Qt::UniqueConnection);

    // Create parameter block
    setParameterBlock(new ParameterBlock(xBlock, this, m_pController));
    m_pCaptionLabel->setCaption(m_pParameterBlock->name());
    m_pCaptionLabel->setExpandable(!m_pParameterBlock->isEmpty());

    // Do connections
    connect(this, &CollapsibleBlock::closedStateChanged, m_pCaptionLabel, &CaptionLabel::onStateChanged, Qt::UniqueConnection);
    connect(m_pCaptionLabel, &CaptionLabel::openOrClose, this, &CollapsibleBlock::onToggleOpenedState, Qt::UniqueConnection);
    connect(m_pCaptionLabel, &CaptionLabel::clearAll, this, &CollapsibleBlock::onClearAll, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock::~CollapsibleBlock()
{
    logInfo("DESTROY COLLAPSIBLE BLOCK");
}

//-------------------------------------------------------------------------------------------------

ParameterBlock *CollapsibleBlock::parameterBlock() const
{
    return m_pParameterBlock;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::setParameterBlock(ParameterBlock *pParameterBlock)
{
    if (pParameterBlock != nullptr)
    {
        // Set parameter block
        m_pParameterBlock = pParameterBlock;
        m_pParameterBlock->setParent(this);

        // Add to layout
        m_pLayout->addWidget(m_pParameterBlock);
        m_pLayout->setAlignment(m_pParameterBlock, Qt::AlignTop);

        // Check closed state
        onOpenOrClose(m_bIsClosed);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onOpenOrClose(bool bClose, bool bRecurse)
{
    if (m_pParameterBlock != nullptr)
    {
        m_bIsClosed = bClose;
        m_pParameterBlock->setVisible(!bClose && m_pParameterBlock->isEnabled());
        emit closedStateChanged(m_bIsClosed);
        if (bRecurse)
        {
            foreach (CollapsibleBlock *pBlock, m_vChildBlocks)
                if (pBlock != nullptr)
                    pBlock->onOpenOrClose(bClose);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onToggleOpenedState()
{
    onOpenOrClose(!m_bIsClosed, false);
}

//-------------------------------------------------------------------------------------------------

bool CollapsibleBlock::isClosed() const
{
    return m_bIsClosed;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::addChildBlock(CollapsibleBlock *pBlock)
{
    if ((pBlock != nullptr) && (!m_vChildBlocks.contains(pBlock)))
    {
        m_vChildBlocks << pBlock;
        pBlock->setParentBlock(this);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onUpdateEnabledState(bool bEnabled)
{
    if (!bEnabled)
        onOpenOrClose(true);
    m_pCaptionLabel->updateEnabledState(bEnabled);
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onClearAll()
{
    // Unselect
    select(false);
    // Clear own parameter block if any
    if (m_pParameterBlock != nullptr)
        // Clear own parameter block
        m_pParameterBlock->clearAll();
    foreach (CollapsibleBlock *pBlock, m_vChildBlocks)
        if (pBlock != nullptr)
            pBlock->onClearAll();
}

//-------------------------------------------------------------------------------------------------

bool CollapsibleBlock::isSelected() const
{
    return m_bIsSelected;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::select(bool bSelect)
{
    m_bIsSelected = bSelect;
    emit blockStatusChanged(this);
    m_pCaptionLabel->setCurrent(bSelect);
    if (!bSelect)
        foreach (CollapsibleBlock *pChildBlock, m_vChildBlocks)
            pChildBlock->select(false);
}

//-------------------------------------------------------------------------------------------------

const QVector<CollapsibleBlock *> &CollapsibleBlock::childBlocks() const
{
    return m_vChildBlocks;
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock *CollapsibleBlock::childBlock(int iRow) const
{
    if ((iRow < 0) || (iRow > (m_vChildBlocks.size()-1)))
        return nullptr;
    return m_vChildBlocks[iRow];
}

//-------------------------------------------------------------------------------------------------

int CollapsibleBlock::row() const
{
    if (m_pParentBlock != nullptr)
    {
        return m_pParentBlock->childBlocks().indexOf(const_cast<CollapsibleBlock *>(this));
    }
    return -1;
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock *CollapsibleBlock::parentBlock() const
{
    return m_pParentBlock;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::setParentBlock(CollapsibleBlock *pParentBlock)
{
    m_pParentBlock = pParentBlock;
}

//-------------------------------------------------------------------------------------------------

CaptionLabel *CollapsibleBlock::captionLabel() const
{
    return m_pCaptionLabel;
}

//-------------------------------------------------------------------------------------------------

const QString &CollapsibleBlock::uid() const
{
    return m_sUID;
}

//-------------------------------------------------------------------------------------------------

bool CollapsibleBlock::isExclusive() const
{
    return (m_pParameterBlock != nullptr) ? m_pParameterBlock->isExclusive() : false;
}

//-------------------------------------------------------------------------------------------------

bool CollapsibleBlock::isEnabled() const
{
    return (m_pParameterBlock != nullptr) ? m_pParameterBlock->isEnabled() : false;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::setBlockVariable()
{
    // Retrieve parameter block
    if (m_pParameterBlock != nullptr)
    {
        QString sSelectionVariable = m_pParameterBlock->selectionVariable();
        Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sSelectionVariable);
        if (pParameter != nullptr)
        {
            // Retrieve selection variable
            QString sType = pParameter->type();
            if (m_bIsSelected)
            {
                if (sType == PROPERTY_STRING)
                    m_pController->parameterMgr()->setParameterValue(sSelectionVariable, m_pParameterBlock->value());
                else
                if (sType == PROPERTY_BOOLEAN)
                    m_pController->parameterMgr()->setParameterValue(sSelectionVariable, PROPERTY_YES);
            }
            else
            {
                if (sType == PROPERTY_BOOLEAN)
                    m_pController->parameterMgr()->setParameterValue(sSelectionVariable, PROPERTY_NO);
            }
        }
    }
}

