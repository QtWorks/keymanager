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

    //! Set license mode
    void setLicenseMode(bool bLicenseMode);

    //! Show application body?
    void showApplicationBody(bool bShowApplicationBody);

    //! Show debug tab?
    void showDebugTab(bool bShowDebugTab);

    //! Show openscad output log?
    void showOpenSCADOutputLog(bool bShowOpenSCADOoutputLog);

    //! Set question
    void setQuestion(const QString &sQuestion);

    //! Set answer
    void setAnswer(const QString &sAnswer);

    //! Setup
    void buildMenus();

protected:
    //! Handle close event
    virtual void closeEvent(QCloseEvent *event);

private:
    //! Export blocks to XML
    void exportBlocksToXML(CXMLNode &xRootNode);

    //! Import block parameters from XML
    void importBlockParametersFromXML(const QString &sInputFile);

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

    //! All blocks
    QHash<QString, CollapsibleBlock *> m_hAllBlocks;

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

    //! Tree visibility changed, update button labels
    void onMenu1TreeVisibilityChanged(bool bTreeVisible);
    void onMenu2TreeVisibilityChanged(bool bTreeVisible);
    void onMenu3TreeVisibilityChanged(bool bTreeVisible);
    void onMenu4TreeVisibilityChanged(bool bTreeVisible);

    //! Validate answer clicked
    void onValidateAnswer(const QString &sAnswer);
};

#endif // MAINWINDOW_H
