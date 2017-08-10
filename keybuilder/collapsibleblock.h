#ifndef COLLAPSIBLEBLOCK_H
#define COLLAPSIBLEBLOCK_H

// Qt
#include <QWidget>
#include <QLabel>
class QVBoxLayout;

// Application
#include "cxmlnode.h"
class CaptionLabel;
class ParameterBlock;
class Controller;

class CollapsibleBlock : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit CollapsibleBlock(const CXMLNode &xBlock, Controller *pController, QWidget *parent=nullptr);

    //! Destructor
    ~CollapsibleBlock();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return own parameter block
    ParameterBlock *parameterBlock() const;

    //! Is selected?
    bool isSelected() const;

    //! Select
    void select(bool bSelect);

    //! Return child blocks
    QVector<CollapsibleBlock *> childBlocks() const;

    //! Return parent block
    CollapsibleBlock *parentBlock() const;

    //! Set parent block
    void setParentBlock(CollapsibleBlock *pParentBlock);

    //! Return label
    CaptionLabel *captionLabel() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Return closed state
    bool isClosed() const;

    //! Add child block
    void addChildBlock(CollapsibleBlock *pBlock);

    //! Process block variable
    void processBlockVariable();

    //! Reset block variable
    void resetBlockVariable();

    //! Full reset
    void fullReset();

private:
    //! Set parameter block
    void setParameterBlock(ParameterBlock *pParameterBlock);

    //! Set current block
    void setCurrentBlock(CollapsibleBlock *pBlock);

    //! Unselect me
    void unselectMe();

private:
    //! Own parameter block
    ParameterBlock *m_pParameterBlock;

    //! Own layout
    QVBoxLayout *m_pLayout;

    //! Caption
    CaptionLabel *m_pCaptionLabel;

    //! Closed?
    bool m_bIsClosed;

    //! Is selected?
    bool m_bIsSelected;

    //! Child blocks
    QVector<CollapsibleBlock *> m_vChildBlocks;

    //! Parent block
    CollapsibleBlock *m_pParentBlock;

    // Controller
    Controller *m_pController;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Close or open
    void onClose(bool bClose, bool bRecurse=true);

    //! Toggle closed state
    void onToggleClosedState();

    //! Update enabled state
    void onUpdateEnabledState(bool bEnabled);

    //! Clear all
    void onClearAll();

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! State changed (closed or opened)
    void closedStateChanged(bool bClosed);

    //! Select me
    void selectMe();
};

#endif /*COLLAPSIBLEBLOCK_H*/
