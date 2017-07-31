#ifndef COLLAPSIBLESTACK_H
#define COLLAPSIBLESTACK_H

// Qt
#include <QWidget>

// Application
class CollapsibleBlock;
class QVBoxLayout;

class CollapsibleStack : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit CollapsibleStack(QWidget *parent=nullptr);

    //! Destructor
    ~CollapsibleStack();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return list of blocks
    const QVector<CollapsibleBlock *> &blocks() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Add block
    CollapsibleBlock *setWidget(const QString &sCaption, QWidget *pWidget, bool bHasParameters);

    //! Expand all
    void expandAll();

    //! Collapse all
    void collapseAll();

private:
    //! Return all collapsed state
    bool allCollapsed() const;

    //! Own layout
    QVBoxLayout *m_pLayout;

    //! Blocks
    QVector<CollapsibleBlock *> m_vBlocks;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Block selected
    void onBlockSelected();

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Block selected
    void blockSelected(CollapsibleBlock *pBlock);
};

#endif // COLLAPSIBLESTACK_H
