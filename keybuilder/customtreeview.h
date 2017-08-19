#ifndef CUSTOMTREEVIEW_H
#define CUSTOMTREEVIEW_H

// Qt
#include <QTreeView>

class CustomTreeView : public QTreeView
{
public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CustomTreeView(QWidget *parent=nullptr);

    //! Destructor
    ~CustomTreeView();
};

#endif // CUSTOMTREEVIEW_H
