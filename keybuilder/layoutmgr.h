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

    //! Add parameter block
    CollapsibleBlock *addBlock(QWidget *pBlock, const QString &sName, bool bIsEmpty);

    //! Set size
    void setSize(int iSize);

    //! Build menu
    virtual void buildMenu(const CXMLNode &xNode);

    //! Build block
    void addCollapsibleBlockToStack(const CXMLNode &xBlock);

protected:
    //! Return all blocks
    QList<CollapsibleBlock *> allBlocks() const;

protected:
    //! Vector of stacks
    QVector<CollapsibleStack *> m_vStacks;

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

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Open all parameter blocks
    void onOpenAll();

    //! Close all parameter blocks
    void onCloseAll();
};

#endif // LAYOUTMGR_H
