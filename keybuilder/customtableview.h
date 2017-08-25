#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

// Qt
#include <QTableView>

class CustomTableView : public QTableView
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CustomTableView(QWidget *parent=nullptr);

    //! Destructor
    ~CustomTableView();

public slots:
    //! Data changed
    void onRowCountChanged();
};

#endif // CUSTOMTABLEVIEW_H
