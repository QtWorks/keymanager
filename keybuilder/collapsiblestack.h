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
    QList<CollapsibleBlock *> blocks() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Add block
    CollapsibleBlock *addBlock(const QString &sCaption, QWidget *pWidget, bool bIsEmpty);

    //! Open all
    void openAll();

    //! Close all
    void closeAll();

private:
    //! Own layout
    QVBoxLayout *m_pLayout;
};

#endif // COLLAPSIBLESTACK_H
