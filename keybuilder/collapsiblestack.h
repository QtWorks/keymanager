#ifndef COLLAPSIBLESTACK_H
#define COLLAPSIBLESTACK_H

// Qt
#include <QWidget>

// Application
#include "cxmlnode.h"
class CollapsibleBlock;
class ParameterBlock;
class QVBoxLayout;
class Controller;

class CollapsibleStack : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit CollapsibleStack(Controller *pController, QWidget *parent=nullptr);

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
    CollapsibleBlock *addBlock(const CXMLNode &xBlock);

    //! Open all
    void openAll();

    //! Close all
    void closeAll();

private:
    //! Own layout
    QVBoxLayout *m_pLayout;

    //! Own blocks
    QVector<CollapsibleBlock *> m_vBlocks;

    //! Controller
    Controller *m_pController;
};

#endif // COLLAPSIBLESTACK_H
