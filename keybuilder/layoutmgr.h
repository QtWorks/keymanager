#ifndef LAYOUTMGR_H
#define LAYOUTMGR_H

// Qt
#include <QWidget>

// Application
#include "cxmlnode.h"
namespace Ui {
class LayoutMgr;
}
class CollapsibleStack;
class CollapsibleBlock;
class Controller;
class ParameterBlock;
class BlockModel;
class SelectionMgr;

class LayoutMgr : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit LayoutMgr(QWidget *parent=nullptr);

    //! Destructor
    virtual ~LayoutMgr();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return controller
    Controller *controller() const;

    //! Set controller
    void setController(Controller *pController);

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Set size
    void setSize(int iSize);

    //! Build menu
    virtual void buildMenu(const CXMLNode &xBlock);

protected:
    //! Vector of stacks
    QVector<CollapsibleStack *> m_vStacks;

private:
    //! Add block to stack
    void addBlockToStack(CollapsibleBlock *pBlock);

    //! Connect blocks to selection mgr
    void connectBlocksToSelectionMgr(CollapsibleBlock *pBlock);

    //! Evaluate enabled condition
    void evaluateEnabledCondition(CollapsibleBlock *pBlock);

private:
    //! UI
    Ui::LayoutMgr *ui;

    //! Number of parameter blocks
    int m_nBlocks;

    //! Size
    int m_iSize;

    //! # columns
    int m_nCols;

    //! # block per stack
    int m_nBlockPerStack;

    //! Controller
    Controller *m_pController;

    //! Root block
    CollapsibleBlock *m_pRootCollapsibleBlock;

    //! Block model
    BlockModel *m_pBlockModel;

    //! Selection manager
    SelectionMgr *m_pSelectionMgr;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Open all parameter blocks
    void onOpenAll();

    //! Close all parameter blocks
    void onCloseAll();

    //! Clear all
    void onClearAll();

    //! Highlight item
    void onHighlightItem(const QModelIndex &index, CollapsibleBlock *pBlock);

    //! Block status changed
    void onBlockSelectionStatusChanged(CollapsibleBlock *pBlock);

    //! Closed state changed
    void onClosedStateChanged(bool bClosed);
};

#endif // LAYOUTMGR_H
