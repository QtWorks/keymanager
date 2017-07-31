#ifndef LAYOUTMGR_H
#define LAYOUTMGR_H

// Qt
#include <QWidget>

// Application
namespace Ui {
class LayoutMgr;
}
class CollapsibleStack;
class CollapsibleBlock;

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
    ~LayoutMgr();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Add key block
    CollapsibleBlock *addBlock(QWidget *pBlock, const QString &sName, bool bHasParameters);

    //! Set size
    void setSize(int iSize);

private:
    //! UI
    Ui::LayoutMgr *ui;

    //! Number of key blocks
    int m_nBlocks;

    //! Vector of stacks
    QVector<CollapsibleStack *> m_vStacks;

    //! Size
    int m_iSize;

    //! # columns
    int m_nCols;

    //! # block per stack
    int m_nBlockPerStack;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Expand all key blocks in menu 1
    void onExpandAll();

    //! Collapse all key blocks in menu 1
    void onCollapseAll();

    //! Block selected
    void onBlockSelected(CollapsibleBlock *pBlock);
};

#endif // LAYOUTMGR_H
