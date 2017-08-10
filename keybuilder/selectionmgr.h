#ifndef SELECTIONMGR_H
#define SELECTIONMGR_H

// Qt
#include <QObject>

// Application
class Controller;
class CollapsibleBlock;

class SelectionMgr : public QObject
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit SelectionMgr(QObject *parent = nullptr);

    //! Destructor
    ~SelectionMgr();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Set controller
    void setController(Controller *pController);

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! A block was selected
    void onBlockSelected();

private:
    //! Unselect block
    void unselectBlock(CollapsibleBlock *pBlock);

    //! Select block
    void selectBlock(CollapsibleBlock *pBlock);

private:
    //! Controller
    Controller *m_pController;
};

#endif // SELECTIONMGR_H
