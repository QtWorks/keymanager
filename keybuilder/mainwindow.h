#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>

// Application
#include "cxmlnode.h"
class Controller;
class CollapsibleBlock;
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

    //! Load CSS
    void loadCSS();

private:
    //! UI
    Ui::MainWindow *ui;

    //! Controller
    Controller *m_pController;

    //! All opened at startup?
    bool m_bAllOpened;

public slots:
    //! Export parameters to SCAD
    void onExportParametersToSCAD();

    //! Export parameters to TXT
    void onExportParametersToTXT();

    //! Import parameters from TXT
    void onImportParametersFromTXT();

    //! Create key
    void onCreateKeyClicked();

    //! Use existing key
    void onUseExistingKeyClicked();

    //! Visualize STL
    void onVisualizeSTLClicked();
};

#endif // MAINWINDOW_H
