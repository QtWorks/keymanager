#ifndef LAYOUTMGR_H
#define LAYOUTMGR_H

// Qt
#include <QWidget>

// Application
namespace Ui {
class LayoutMgr;
}
class CollapsibleStack;
class CollapsiblePanel;

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
    void addBlock(QWidget *pBlock, const QString &sName);

private:
    //! UI
    Ui::LayoutMgr *ui;

    //! Number of key blocks
    int m_nKeyBlocks;

    //! Vector of stacks
    QVector<CollapsibleStack *> m_vStacks;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Expand all key blocks in menu 1
    void onExpandAll();

    //! Collapse all key blocks in menu 1
    void onCollapseAll();

    //! Panel selected
    void onPanelSelected(CollapsiblePanel *pPanel);
};

#endif // LAYOUTMGR_H
