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
#include "selectionmgr.h"

//-------------------------------------------------------------------------------------------------

CollapsibleBlock::CollapsibleBlock(const CXMLNode &xBlock, Controller *pController, QWidget *parent) : QWidget(parent),
    m_pParameterBlock(nullptr), m_bIsClosed(true), m_bIsSelected(false), m_pParentBlock(nullptr), m_pController(pController)
{
    // Set size policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Create layout
    m_pLayout = new QVBoxLayout(this);

    // Create caption label
    m_pCaptionLabel = new CaptionLabel(this);
    m_pLayout->addWidget(m_pCaptionLabel);
    connect(m_pCaptionLabel, &CaptionLabel::selectMe, this, &CollapsibleBlock::selectMe, Qt::UniqueConnection);
    connect(this, &CollapsibleBlock::selectMe, m_pController->selectionMgr(), &SelectionMgr::onBlockSelected, Qt::UniqueConnection);

    // Create parameter block
    setParameterBlock(new ParameterBlock(xBlock, this, m_pController));
    m_pCaptionLabel->setCaption(m_pParameterBlock->name());
    m_pCaptionLabel->setExpandable(!m_pParameterBlock->isEmpty());

    // Do connections
    connect(this, &CollapsibleBlock::closedStateChanged, m_pCaptionLabel, &CaptionLabel::onStateChanged, Qt::UniqueConnection);
    connect(m_pCaptionLabel, &CaptionLabel::openClose, this, &CollapsibleBlock::onOpenClose, Qt::UniqueConnection);
    connect(m_pCaptionLabel, &CaptionLabel::clearAll, this, &CollapsibleBlock::onClearAll, Qt::UniqueConnection);
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
            foreach (CollapsibleBlock *pBlock, m_vChildBlocks)
                if (pBlock != nullptr)
                    pBlock->onClose(bClose);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onOpenClose()
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
    if ((pBlock != nullptr) && (!m_vChildBlocks.contains(pBlock)))
    {
        m_vChildBlocks << pBlock;
        pBlock->setParentBlock(this);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onUpdateEnabledState(bool bEnabled)
{
    m_pCaptionLabel->updateEnabledState(bEnabled);
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onClearAll()
{
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
    m_pCaptionLabel->setCurrent(bSelect);
    m_pCaptionLabel->repaint();
    if (!bSelect)
        foreach (CollapsibleBlock *pChildBlock, m_vChildBlocks)
            pChildBlock->select(false);
}

//-------------------------------------------------------------------------------------------------

QVector<CollapsibleBlock *> CollapsibleBlock::childBlocks() const
{
    return m_vChildBlocks;
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

void CollapsibleBlock::processBlockVariable()
{
    // Retrieve parameter block
    if (m_pParameterBlock != nullptr)
    {
        // Retrieve selection variable
        QString sSelectionVariable = m_pParameterBlock->selectionVariable();
        Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sSelectionVariable);
        if (pParameter != nullptr)
        {
            QString sType = pParameter->type();
            if (sType == PROPERTY_STRING)
                m_pController->parameterMgr()->setParameterValue(sSelectionVariable, m_pParameterBlock->value());
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
                                    m_pController->parameterMgr()->setParameterValue(sSelectionVariable, pChildBlock == this ? PROPERTY_YES : PROPERTY_NO);
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

void CollapsibleBlock::resetBlockVariable()
{
    // Retrieve selection variable
    if (m_pParameterBlock != nullptr)
    {
        QString sSelectionVariable = m_pParameterBlock->selectionVariable();
        Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sSelectionVariable);
        if (pParameter != nullptr)
            pParameter->resetToDefaultValue();
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::fullReset()
{
    resetBlockVariable();
    if (m_pParameterBlock != nullptr)
        m_pParameterBlock->clearAll();
    foreach (CollapsibleBlock *pChildBlock, m_vChildBlocks)
        if (pChildBlock != nullptr)
            pChildBlock->fullReset();
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::unselectMe()
{
    select(false);
    onClearAll();
    foreach (CollapsibleBlock *pChildBlock, m_vChildBlocks)
        pChildBlock->unselectMe();
}
