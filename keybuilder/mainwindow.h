#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>

// Application
#include "cxmlnode.h"

class Controller;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:    
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit MainWindow(QWidget *parent=nullptr);

    //! Destructor
    ~MainWindow();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Set controller
    void setController(Controller *pController);

    //! Build menu 1 tab
    void buildMenu1Tab(const CXMLNode &xNode);

    //! Build menu 2 tab
    void buildMenu2Tab(const CXMLNode &xNode);

private:
    //! UI
    Ui::MainWindow *ui;

    //! Controller
    Controller *m_pController;
};

#endif // MAINWINDOW_H
