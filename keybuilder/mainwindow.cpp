// Qt
#include <QDebug>
#include <QFileDialog>
#include <QProcess>

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
#define HOME_TAB 0
#define MENU1_TAB 1
#define MENU2_TAB 2
#define MENU3_TAB 3
#define SETTINGS_TAB 4
#define VISUALIZE_STL_TAB 5
#define OUTPUT_SCAD_TAB 6

//-------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow), m_bAllOpened(false), m_pSTLWindow(new STLWindow),
    m_sNextSTLFileToDisplay("")
{    
    // Set window title
    setWindowTitle(tr("OFC3DKEY V.2.0.2017"));

    // Setup timer
    m_STLViewerTimer.setInterval(500);
    m_STLViewerTimer.setSingleShot(true);
    connect(&m_STLViewerTimer, &QTimer::timeout, this, &MainWindow::onSTLViewerTimerTimeOut);

    // Load CSS
    loadCSS();

    // Setup UI
    ui->setupUi(this);

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

    // Set controller on layout managers
    ui->menu1LayoutMgr->setController(m_pController);
    ui->menu2LayoutMgr->setController(m_pController);
    ui->menu3LayoutMgr->setController(m_pController);
    ui->menuSettingsLayoutMgr->setController(m_pController);

    // Close/Open all
    connect(ui->closeAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);
    connect(ui->generateSTLButtonMenu1, &QPushButton::clicked, this, &MainWindow::onGenerateSTL);
    connect(ui->saveKeyParametersButtonMenu1, &QPushButton::clicked, this, &MainWindow::onSaveKeyParameters);

    connect(ui->closeAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);
    connect(ui->generateSTLButtonMenu2, &QPushButton::clicked, this, &MainWindow::onGenerateSTL);
    connect(ui->saveKeyParametersButtonMenu2, &QPushButton::clicked, this, &MainWindow::onSaveKeyParameters);

    connect(ui->closeAllButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);
    connect(ui->generateSTLButtonMenu3, &QPushButton::clicked, this, &MainWindow::onGenerateSTL);
    connect(ui->saveKeyParametersButtonMenu3, &QPushButton::clicked, this, &MainWindow::onSaveKeyParameters);

    connect(ui->importParametersButton, &QPushButton::clicked, this, &MainWindow::onImportParametersFromTXT, Qt::UniqueConnection);

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
        ui->tabWidget->removeTab(OUTPUT_SCAD_TAB);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::loadCSS()
{
    QString sStyle = Utils::loadFile(":/css/main.css");
    this->setStyleSheet(sStyle);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onImportParametersFromTXT()
{
    QString sInputFileName = QFileDialog::getOpenFileName(this, tr("Select input TXT parameter file"), QCoreApplication::applicationDirPath(), tr("TXT (*.txt)"));
    if (!sInputFileName.isEmpty())
        m_pController->importParametersFromTXT(sInputFileName);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onCreateKeyClicked()
{
    ui->tabWidget->setCurrentIndex(MENU1_TAB);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onUseExistingKeyClicked()
{
    ui->tabWidget->setCurrentIndex(MENU2_TAB);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onVisualizeSTLClicked()
{
    QString sProgram = "C:/Program Files/OpenSCAD/openscad.exe -o out.stl -D 'quality=\"production\" D:/projects/keymanager/bin/toto.scad";
    qDebug() << sProgram;
    QProcess *pProcess = new QProcess(this);
    QStringList args;
    pProcess->start(sProgram, args);
    ui->tabWidget->setCurrentIndex(VISUALIZE_STL_TAB);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onGenerateSTL()
{
    // Step 1: do replacement in script_in.scad
    m_pController->generateSTL();
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onSaveKeyParameters()
{
    QString sOutputFileName = QFileDialog::getSaveFileName(this, tr("Select output key parameters file"), QCoreApplication::applicationDirPath(), tr("TXT (*.txt)"));
    if (!sOutputFileName.isEmpty())
        m_pController->exportParametersToTXT(sOutputFileName);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onSTLFileReady(const QString &sSTLFilePath)
{
    // Load STL
    m_sNextSTLFileToDisplay = sSTLFilePath;
    m_STLViewerTimer.start();
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onOutputSCADReady(const QString &sOutputSCADFile)
{
    QString sOutputSCADContents = Utils::loadFile(sOutputSCADFile);
    if (!sOutputSCADContents.isEmpty() && (m_pController->debugOn()))
    {
        ui->plainTextEdit->document()->setPlainText(sOutputSCADContents);
    }
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onSTLViewerTimerTimeOut()
{
    // Go to STL tab
    ui->tabWidget->setCurrentIndex(VISUALIZE_STL_TAB);

    // Display
    m_pSTLWindow->load_stl(m_sNextSTLFileToDisplay);
}
