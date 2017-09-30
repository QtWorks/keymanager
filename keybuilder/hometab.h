#ifndef HOMETAB_H
#define HOMETAB_H

// Qt
#include <QWidget>
#include <QFont>

// Application
#include "cxmlnode.h"
#include "descriptiontaggedwidget.h"

namespace Ui {
class HomeTab;
}

class HomeTab : public DescriptionTaggedWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit HomeTab(QWidget *parent=nullptr);

    //! Destructor
    ~HomeTab();

protected:
    //! Paint event
    virtual void paintEvent(QPaintEvent *event);

private:
    //! UI
    Ui::HomeTab *ui;
};

#endif // HOMETAB_H
