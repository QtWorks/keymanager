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

    //! Add parameter block
    CollapsibleBlock *addBlock(QWidget *pBlock, const QString &sName, bool bIsEmpty);

    //! Set size
    void setSize(int iSize);

    //! Build menu
    void buildMenu(const CXMLNode &xNode);

    //! Build block
    void addCollapsibleBlockToStack(const CXMLNode &xBlock);

private:
    //! UI
    Ui::LayoutMgr *ui;

    //! Number of parameter blocks
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

    //! Open all parameter blocks
    void onOpenAll();

    //! Close all parameter blocks
    void onCloseAll();

    //! Block selected
    void onBlockSelected(CollapsibleBlock *pBlock);

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);
};

#endif // LAYOUTMGR_H
