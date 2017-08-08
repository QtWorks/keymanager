// Qt
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>

// Application
#include "collapsibleblock.h"
#include "captionlabel.h"
#include "constants.h"
#include "parameterblock.h"
#include "controller.h"
#include "parametermgr.h"

//-------------------------------------------------------------------------------------------------

CollapsibleBlock::CollapsibleBlock(const CXMLNode &xBlock, Controller *pController, QWidget *parent) : QWidget(parent),
    m_pParameterBlock(nullptr), m_bIsClosed(true), m_bIsSelected(false), m_pParentBlock(nullptr), m_pController(pController)
{
    // Set size policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Create layout
    m_pLayout = new QVBoxLayout(this);

    // Create caption label
    m_pLabel = new CaptionLabel(this);
    m_pLayout->addWidget(m_pLabel);
    m_pLabel->setBlock(this);

    // Create parameter block
    setParameterBlock(new ParameterBlock(xBlock, this, m_pController));
    m_pLabel->setCaption(m_pParameterBlock->name());
    m_pLabel->setExpandable(!m_pParameterBlock->isEmpty());

    // Do connections
    connect(this, &CollapsibleBlock::closedStateChanged, m_pLabel, &CaptionLabel::onStateChanged);
    connect(m_pLabel, &CaptionLabel::toggleClosedState, this, &CollapsibleBlock::onToggleClosedState);
    connect(m_pLabel, &CaptionLabel::clearAll, this, &CollapsibleBlock::onClearAll);
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock::~CollapsibleBlock()
{
    qDebug() << "INFORMATION: DESTROY COLLAPSIBLE BLOCK";
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
        onClose(m_bIsClosed);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onClose(bool bClose, bool bRecurse)
{
    if (m_pParameterBlock != nullptr)
    {
        m_bIsClosed = bClose;
        m_pParameterBlock->setVisible(!bClose && m_pParameterBlock->isEnabled());
        emit closedStateChanged(m_bIsClosed);
        if (bRecurse)
        {
            foreach (CollapsibleBlock *pBlock, m_vBlocks)
                if (pBlock != nullptr)
                    pBlock->onClose(bClose);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onToggleClosedState()
{
    onClose(!m_bIsClosed, false);
}

//-------------------------------------------------------------------------------------------------

bool CollapsibleBlock::isClosed() const
{
    return m_bIsClosed;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::addChildBlock(CollapsibleBlock *pBlock)
{
    if ((pBlock != nullptr) && (!m_vBlocks.contains(pBlock)))
    {
        m_vBlocks << pBlock;
        pBlock->setParentBlock(this);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onUpdateEnabledState(bool bEnabled)
{
    m_pLabel->updateEnabledState(bEnabled);
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onClearAll()
{
    // Clear own parameter block if any
    if (m_pParameterBlock != nullptr)
        // Clear own parameter block
        m_pParameterBlock->clearAll();
    foreach (CollapsibleBlock *pBlock, m_vBlocks)
        if (pBlock != nullptr)
            pBlock->onClearAll();
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onSelectMe()
{
    // Set current block
    setCurrentBlock(this);

    // Set block variable
    processBlockVariable(this);

    // No selected, make sure no child is selected
    if (!m_bIsSelected)
        unselectMe();
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
    m_pLabel->setCurrent(bSelect);
    m_pLabel->repaint();
}

//-------------------------------------------------------------------------------------------------

QVector<CollapsibleBlock *> CollapsibleBlock::childBlocks() const
{
    return m_vBlocks;
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

void CollapsibleBlock::setCurrentBlock(CollapsibleBlock *pBlock)
{
    if ((m_pParentBlock != nullptr) || (m_pParameterBlock != nullptr))
    {
        ParameterBlock *pParentParameterBlock = m_pParentBlock->parameterBlock();
        if (pParentParameterBlock != nullptr)
        {
            // Is parent exclusive?
            bool bParentIsExclusive = pParentParameterBlock->isExclusive();

            // Get owner collapsible block
            QVector<CollapsibleBlock *> vChildBlocks = m_pParentBlock->childBlocks();
            foreach (CollapsibleBlock *pChildBlock, vChildBlocks)
            {
                if (bParentIsExclusive)
                    pChildBlock->select(pChildBlock == pBlock);
                else
                    if (pChildBlock == pBlock)
                        pChildBlock->select(!pChildBlock->isSelected());
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::processBlockVariable(CollapsibleBlock *pBlock)
{
    if (pBlock != nullptr)
    {
        // Retrieve parameter block
        ParameterBlock *pParameterBlock = pBlock->parameterBlock();
        if (pParameterBlock != nullptr)
        {
            // Retrieve selection variable
            QString sSelectionVariable = pParameterBlock->selectionVariable();
            Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sSelectionVariable);
            if (pParameter != nullptr)
            {
                QString sType = pParameter->type();
                if (sType == PROPERTY_STRING)
                    m_pController->parameterMgr()->setParameterValue(sSelectionVariable, pParameterBlock->value());
                else
                if (sType == PROPERTY_BOOLEAN)
                {
                    // Retrieve parent
                    if (m_pParentBlock != nullptr)
                    {
                        // Retrieve parent parameter block
                        ParameterBlock *pParentParameterBlock = m_pParentBlock->parameterBlock();
                        if (pParentParameterBlock != nullptr)
                        {
                            // Exclusive?
                            bool bParentIsExclusive = pParentParameterBlock->isExclusive();
                            if (!bParentIsExclusive)
                            {
                                QString sValue = pParameter->value();
                                m_pController->parameterMgr()->setParameterValue(sSelectionVariable, sValue == PROPERTY_NO ? PROPERTY_YES : PROPERTY_NO);
                            }
                            else
                            {
                                QVector<CollapsibleBlock *> vChildBlocks = m_pParentBlock->childBlocks();
                                foreach (CollapsibleBlock *pChildBlock, vChildBlocks)
                                {
                                    ParameterBlock *pChildParameterBlock = pChildBlock->parameterBlock();
                                    if (pChildParameterBlock != nullptr)
                                    {
                                        QString sSelectionVariable = pChildParameterBlock->selectionVariable();
                                        m_pController->parameterMgr()->setParameterValue(sSelectionVariable, pBlock == pChildBlock ? PROPERTY_YES : PROPERTY_NO);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::unselectMe()
{
    select(false);
    onClearAll();
    foreach (CollapsibleBlock *pChildBlock, m_vBlocks)
        pChildBlock->unselectMe();
}
