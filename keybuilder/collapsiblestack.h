#ifndef COLLAPSIBLESTACK_H
#define COLLAPSIBLESTACK_H

// Qt
#include <QWidget>

// Application
#include "cxmlnode.h"
class CollapsibleBlock;
class ParameterBlock;
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
    QVector<CollapsibleBlock *> childBlocks() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Add block
    void addBlock(CollapsibleBlock *pBlock);

    //! Open all
    void openAll();

    //! Close all
    void closeAll();

private:
    //! Own layout
    QVBoxLayout *m_pLayout;

    //! Own blocks
    QVector<CollapsibleBlock *> m_vBlocks;
};

#endif // COLLAPSIBLESTACK_H
