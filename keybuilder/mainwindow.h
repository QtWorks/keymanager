#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>

// Application
#include "cxmlnode.h"
class Controller;
class CollapsibleBlock;
class STLWindow;

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

protected:
    //! Handle close event
    virtual void closeEvent(QCloseEvent *event);

private:
    //! UI
    Ui::MainWindow *ui;

    //! Controller
    Controller *m_pController;

    //! All opened at startup?
    bool m_bAllOpened;

    //! STL window
    STLWindow *m_pSTLWindow;

    //! Next STL file to display
    QString m_sNextSTLFileToDisplay;

    //! App dirty?
    bool m_bAppIsDirty;

public slots:
    //! Create key
    void onCreateKeyClicked();

    //! Use existing key
    void onUseExistingKeyClicked();

    //! Visualize STL
    void onVisualizeSTLClicked();

    //! Generate STL
    void onGenerateSTL();

    //! Save key parameters
    void onSaveKeyParameters();

    //! Save generated STL
    void onSaveGeneratedSTL();

    //! STL file ready
    void onSTLFileReady(const QString &sSTLFilePath);

    //! STL file error
    void onSTLFileError(const QString &sSTLFilePath);

    //! Output SCAD ready
    void onOutputSCADReady(const QString &sOutputSCADFile);

    //! Listen to OpenSCAD process status
    void onOpenSCADProcessComplete(const QString &sStatus);
    void onOpenSCADStandardErrorReady(const QString &sStatus);
    void onOpenSCADStandardOutputReady(const QString &sStatus);
};

#endif // MAINWINDOW_H
