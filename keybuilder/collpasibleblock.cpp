// Qt
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDebug>

// Application
#include "collapsibleblock.h"
#include "captionlabel.h"
#include "constants.h"
#include "parameterblock.h"

//-------------------------------------------------------------------------------------------------

CollapsibleBlock::CollapsibleBlock(QWidget *pWidget, const QString &sCaption, bool bIsEmpty, QWidget *parent)
    : QWidget(parent), m_pWidget(nullptr), m_bIsClosed(true), m_bIsCurrent(false)
{
    // Set object name
    setObjectName(COLLAPSIBLEBLOCK_OBJECT_NAME);

    // Set size policy
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Create layout
    m_pLayout = new QVBoxLayout(this);

    // Create caption label
    m_pLabel = new CaptionLabel(this);
    m_pLabel->setCaption(sCaption);
    m_pLabel->setExpandable(!bIsEmpty);
    m_pLayout->addWidget(m_pLabel);

    // Do connections
    connect(m_pLabel, &CaptionLabel::blockSelected, this, &CollapsibleBlock::blockSelected);
    connect(this, &CollapsibleBlock::closedStateChanged, m_pLabel, &CaptionLabel::onStateChanged);
    connect(m_pLabel, &CaptionLabel::toggleClosedState, this, &CollapsibleBlock::onToggleClosedState);

    // Set widget
    setWidget(pWidget);
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock::~CollapsibleBlock()
{
    //qDebug() << "*** DESTROY COLLAPSIBLE BLOCK ***";
}

//-------------------------------------------------------------------------------------------------

QWidget *CollapsibleBlock::widget() const
{
    return m_pWidget;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::setWidget(QWidget *widget)
{
    if (m_pWidget == nullptr)
    {
        m_pWidget = widget;
        m_pWidget->setParent(this);
        m_pLayout->addWidget(m_pWidget);
        m_pLayout->setAlignment(m_pWidget, Qt::AlignTop);
        onClose(m_bIsClosed);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onClose(bool bClose)
{
    if (m_pWidget != nullptr)
    {
        m_bIsClosed = bClose;
        m_pWidget->setVisible(!bClose && m_pWidget->isEnabled());
        emit closedStateChanged(m_bIsClosed);
    }
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onToggleClosedState()
{
    onClose(!m_bIsClosed);
}

//-------------------------------------------------------------------------------------------------

bool CollapsibleBlock::isClosed() const
{
    return m_bIsClosed;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::onUpdateEnabledState(bool bEnabled)
{
    m_pLabel->updateEnabledState(bEnabled);
}

//-------------------------------------------------------------------------------------------------

bool CollapsibleBlock::isCurrent() const
{
    return m_bIsCurrent;
}

//-------------------------------------------------------------------------------------------------

void CollapsibleBlock::setCurrent(bool bCurrent)
{
    m_bIsCurrent = bCurrent;
    m_pLabel->setCurrent(bCurrent);
    m_pLabel->update();
}

//-------------------------------------------------------------------------------------------------

bool CollapsibleBlock::isExclusive() const
{
    ParameterBlock *pParameterBlock = dynamic_cast<ParameterBlock *>(m_pWidget);
    if (pParameterBlock != nullptr)
        return pParameterBlock->isExclusive();
    return true;
}

//-------------------------------------------------------------------------------------------------

QList<CollapsibleBlock *> CollapsibleBlock::blocks() const
{
    return findChildren<CollapsibleBlock *>(COLLAPSIBLEBLOCK_OBJECT_NAME);
}

