// Qt
#include <QDebug>
#include <QFileDialog>
#include <QProcess>
#include <QCloseEvent>
#include <QMessageBox>

// Application
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller.h"
#include "parameterblock.h"
#include "constants.h"
#include "collapsibleblock.h"
#include "parametermgr.h"
#include "utils.h"
#include <src/stlwindow.h>
#include "helper.h"
#define HOME_TAB 0
#define MENU1_TAB 1
#define MENU2_TAB 2
#define MENU3_TAB 3
#define SETTINGS_TAB 4
#define VISUALIZE_STL_TAB 5
#define OUTPUT_SCAD_TAB 6
#define APP_NAME "appname"
#define APP_VERSION "appversion"

//-------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow), m_bAllOpened(false), m_pSTLWindow(new STLWindow),
    m_sNextSTLFileToDisplay(""), m_bAppIsDirty(false)
{    
    // Load CSS
    loadCSS();

    // Setup UI
    ui->setupUi(this);
    ui->progressBar->setRange(0, 0);
    ui->progressBar->setVisible(false);

    // Set window title
    CXMLNode xRootNode = CXMLNode::loadXMLFromFile(":/data/about.xml");
    QString sAppName = xRootNode.attributes()[APP_NAME];
    QString sAppVersion = xRootNode.attributes()[APP_VERSION];
    QString sWindowTitle = sAppName + QString(" ") + sAppVersion;
    setWindowTitle(sWindowTitle);

    // Set window icon
    setWindowIcon(QIcon(":/icons/ico-key.ico"));

    // Add STL Window
    ui->stlViewerLayout->addWidget(m_pSTLWindow);

    // Page navigation
    connect(ui->createKeyButton, &QPushButton::clicked, this, &MainWindow::onCreateKeyClicked, Qt::UniqueConnection);
    connect(ui->useExistingKeyButton, &QPushButton::clicked, this, &MainWindow::onUseExistingKeyClicked, Qt::UniqueConnection);
    connect(ui->visualizeSTLButton, &QPushButton::clicked, this, &MainWindow::onVisualizeSTLClicked, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void MainWindow::setController(Controller *pController)
{
    // Set controller
    m_pController = pController;

    // Listen to output SCAD ready
    connect(m_pController, &Controller::outputSCADReady, this, &MainWindow::onOutputSCADReady, Qt::UniqueConnection);

    // Listen to STLFileReady signal
    connect(m_pController, &Controller::STLFileReady, this, &MainWindow::onSTLFileReady, Qt::UniqueConnection);

    // Listen to STLFileError signal
    connect(m_pController, &Controller::STLFileError, this, &MainWindow::onSTLFileError, Qt::UniqueConnection);

    // Listen to OpenSCAD process complete
    connect(m_pController, &Controller::openSCADProcessComplete, this, &MainWindow::onOpenSCADProcessComplete, Qt::UniqueConnection);

    // Listen to OpenSCAD standard output ready
    connect(m_pController, &Controller::openSCADStandardOutputReady, this, &MainWindow::onOpenSCADStandardOutputReady, Qt::UniqueConnection);

    // Listen to OpenSCAD standard error ready
    connect(m_pController, &Controller::openSCADStandardErrorReady, this, &MainWindow::onOpenSCADStandardErrorReady, Qt::UniqueConnection);

    // Set controller on layout managers
    ui->menu1LayoutMgr->setController(m_pController);
    ui->menu2LayoutMgr->setController(m_pController);
    ui->menu3LayoutMgr->setNumberOfStacks(1);
    ui->menu3LayoutMgr->setController(m_pController);
    ui->menuSettingsLayoutMgr->setController(m_pController);

    // Close/Open all
    connect(ui->closeAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);
    connect(ui->generateSTLButtonMenu1, &QPushButton::clicked, this, &MainWindow::onGenerateSTL, Qt::UniqueConnection);
    connect(ui->saveKeyParametersButtonMenu1, &QPushButton::clicked, this, &MainWindow::onSaveKeyParameters, Qt::UniqueConnection);

    connect(ui->closeAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);
    connect(ui->generateSTLButtonMenu2, &QPushButton::clicked, this, &MainWindow::onGenerateSTL, Qt::UniqueConnection);
    connect(ui->saveKeyParametersButtonMenu2, &QPushButton::clicked, this, &MainWindow::onSaveKeyParameters, Qt::UniqueConnection);

    connect(ui->closeAllButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);
    connect(ui->generateSTLButtonMenu3, &QPushButton::clicked, this, &MainWindow::onGenerateSTL, Qt::UniqueConnection);
    connect(ui->saveKeyParametersButtonMenu3, &QPushButton::clicked, this, &MainWindow::onSaveKeyParameters, Qt::UniqueConnection);

    // Build menu 1 tab
    ui->menu1LayoutMgr->buildMenu(m_pController->menu1Node());

    // Build menu 2 tab
    ui->menu2LayoutMgr->buildMenu(m_pController->menu2Node());
    Parameter *pTypeOfKeyParameter = m_pController->parameterMgr()->getParameterByVariableName(VARIABLE_TYPE_OF_KEY);
    if (pTypeOfKeyParameter != nullptr)
    {
        connect(pTypeOfKeyParameter, &Parameter::parameterValueChanged, ui->menu2LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);
        connect(pTypeOfKeyParameter, &Parameter::parameterValueChanged, ui->menu3LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);
    }

    // Build menu 3 tab
    ui->menu3LayoutMgr->buildMenu(m_pController->menu3Node());

    // Build settings tab
    ui->menuSettingsLayoutMgr->buildMenu(m_pController->settingsNode());

    // SCAD output tab
    if (!m_pController->debugOn())
    {
        ui->tabWidget->removeTab(OUTPUT_SCAD_TAB);
        ui->textBrowser->hide();
    }
}

//-------------------------------------------------------------------------------------------------

void MainWindow::loadCSS()
{
    QString sStyle = Utils::loadFile(":/css/main.css");
    this->setStyleSheet(sStyle);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onCreateKeyClicked()
{
    ui->tabWidget->setCurrentIndex(MENU1_TAB);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onUseExistingKeyClicked()
{
    QString sInputFileName = QFileDialog::getOpenFileName(this, tr("Select input TXT parameter file"), QCoreApplication::applicationDirPath(), tr("TXT (*.txt)"));
    if (!sInputFileName.isEmpty())
        m_pController->importParametersFromTXT(sInputFileName);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onVisualizeSTLClicked()
{
    ui->tabWidget->setCurrentIndex(VISUALIZE_STL_TAB);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onGenerateSTL()
{
    ui->textBrowser->clear();

    // Step 1: do replacement in script_in.scad
    QString sOpenSCADPath = Utils::openSCADPath();
    if (!sOpenSCADPath.isEmpty())
    {
        m_pController->generateSTL();
    }
    else
    {
        QString sMsg("OPENSCAD NOT FOUND ON THIS SYSTEM");
        ui->statusbar->showMessage(sMsg);
    }
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onSaveKeyParameters()
{
    QString sOutputFileName = QFileDialog::getSaveFileName(this, tr("Select output key parameters file"), QCoreApplication::applicationDirPath(), tr("TXT (*.txt)"));
    if (!sOutputFileName.isEmpty())
        m_pController->exportParametersToTXT(sOutputFileName);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onSaveGeneratedSTL()
{
    QString sLastGeneratedSTLFile = m_pController->nextOutputSTLFile();
    QFileInfo fi(sLastGeneratedSTLFile);
    if (fi.exists())
    {
        QString sOutputFileName = QFileDialog::getSaveFileName(this, tr("Select output STL file"), QCoreApplication::applicationDirPath(), tr("STL (*.stl)"));
        if (!sOutputFileName.isEmpty())
        {
            QFile::copy(sLastGeneratedSTLFile, sOutputFileName);
        }
    }
    else
    {
        QString sMsg = QString("FILE %1 DOES NOT EXIST ANYMORE").arg(sLastGeneratedSTLFile);
        logError(sMsg);
    }
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onSTLFileReady(const QString &sSTLFilePath)
{
    // Hide progress bar
    ui->progressBar->setVisible(false);

    // Load STL
    QFileInfo fi(sSTLFilePath);
    if (fi.exists())
    {
        QString sMsg = QString("STL FILE READY AT: %1").arg(sSTLFilePath);
        logInfo(sMsg);
        m_sNextSTLFileToDisplay = sSTLFilePath;

        // Go to STL viewer tab
        ui->tabWidget->setCurrentIndex(VISUALIZE_STL_TAB);

        // Display
        m_pSTLWindow->load_stl(m_sNextSTLFileToDisplay);
        m_pSTLWindow->viewOrthographic();
    }
    else
    {
        QString sMsg = QString("COULD NOT FIND STL FILE AT: %1").arg(sSTLFilePath);
        logError(sMsg);
    }
    m_bAppIsDirty = true;
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onSTLFileError(const QString &sErrorMsg)
{
    // Hide progress bar
    ui->progressBar->setVisible(false);

    logError(sErrorMsg);
    ui->statusbar->showMessage(sErrorMsg);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onOutputSCADReady(const QString &sOutputSCADFile)
{
    QString sOutputSCADContents = Utils::loadFile(sOutputSCADFile);
    if (!sOutputSCADContents.isEmpty() && (m_pController->debugOn()))
    {
        ui->progressBar->setVisible(true);
        QString sMsg = "BUILDING STL...";
        logInfo(sMsg);
        statusBar()->showMessage(sMsg);
        ui->plainTextEdit->load(sOutputSCADContents);
    }
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onOpenSCADProcessComplete(const QString &sStatus)
{
    ui->progressBar->setVisible(false);
    QString sMsg = "STL BUILD SUCCESS";
    logInfo(sMsg);
    statusBar()->showMessage("STL BUILD SUCCESS");
    ui->textBrowser->append(sStatus);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onOpenSCADStandardErrorReady(const QString &sStatus)
{
    ui->progressBar->setVisible(false);
    QString sMsg = "STL BUILD FAILURE";
    logInfo(sMsg);
    statusBar()->showMessage("STL BUILD FAILURE");
    ui->textBrowser->append(sStatus);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onOpenSCADStandardOutputReady(const QString &sStatus)
{
    ui->progressBar->setVisible(false);
    statusBar()->showMessage("");
    ui->textBrowser->append(sStatus);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_bAppIsDirty)
    {
        // Save key parameters
        QMessageBox msgBox;
        msgBox.setText("Save Key Parameters?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int iValue = msgBox.exec();
        if (iValue == QMessageBox::Yes)
            onSaveKeyParameters();

        // Save generated STL
        msgBox.setText("Save generated STL?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        iValue = msgBox.exec();
        if (iValue == QMessageBox::Yes)
            onSaveGeneratedSTL();
    }
    event->accept();
}
