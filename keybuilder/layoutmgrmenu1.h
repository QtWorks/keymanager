#ifndef LAYOUTMGRMENU1_H
#define LAYOUTMGRMENU1_H

// Application
#include "layoutmgr.h"

class LayoutMgrMenu1 : public LayoutMgr
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    LayoutMgrMenu1(QWidget *parent=nullptr);

    //! Destructor
    virtual ~LayoutMgrMenu1();

    //! Build menu
    virtual void buildMenu(const CXMLNode &xNode);

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Block selected
    void onBlockSelected();
};

#endif // LAYOUTMGRMENU1_H
