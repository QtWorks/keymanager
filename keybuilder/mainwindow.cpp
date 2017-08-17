// Qt
#include <QDebug>
#include <QFileDialog>

// Application
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller.h"
#include "parameterblock.h"
#include "constants.h"
#include "collapsibleblock.h"
#include "parametermgr.h"
#include <src/stlwindow.h>
#define CREATE_KEY_TAB 1
#define USE_EXISTING_KEY_TAB 2
#define VISUALIZE_STL_TAB 5

//-------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    // Set window title
    setWindowTitle(tr("OFC3DKEY V.2.0.2017"));

    // Load CSS
    loadCSS();

    // Setup UI
    ui->setupUi(this);

    // Add STL Window
    ui->stlViewerLayout->addWidget(new STLWindow);

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

    // Set controller on layout managers
    ui->menu1LayoutMgr->setController(m_pController);
    ui->menu2LayoutMgr->setController(m_pController);
    ui->menu3LayoutMgr->setController(m_pController);
    ui->menuSettingsLayoutMgr->setController(m_pController);

    // Close/Open all
    connect(ui->closeAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);

    connect(ui->closeAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu2, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);

    connect(ui->closeAllButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu3, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);

    connect(ui->exportToSCADButton, &QPushButton::clicked, this, &MainWindow::onExportParametersToSCAD, Qt::UniqueConnection);
    connect(ui->exportParametersButton, &QPushButton::clicked, this, &MainWindow::onExportParametersToTXT, Qt::UniqueConnection);
    connect(ui->importParametersButton, &QPushButton::clicked, this, &MainWindow::onImportParametersFromTXT, Qt::UniqueConnection);

    // Build menu 1 tab
    ui->menu1LayoutMgr->buildMenu(m_pController->menu1Node());

    // Build menu 2 tab
    ui->menu2LayoutMgr->buildMenu(m_pController->menu2Node());
    Parameter *pTypeOfKeyParameter = m_pController->parameterMgr()->getParameterByVariableName(VARIABLE_TYPE_OF_KEY);
    if (pTypeOfKeyParameter != nullptr)
        connect(pTypeOfKeyParameter, &Parameter::parameterValueChanged, ui->menu2LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);

    // Build menu 3 tab
    ui->menu3LayoutMgr->buildMenu(m_pController->menu3Node());

    // Build settings tab
    ui->menuSettingsLayoutMgr->buildMenu(m_pController->settingsNode());
}

//-------------------------------------------------------------------------------------------------

void MainWindow::loadCSS()
{
    // Set style sheet
    QFile file(":/css/main.css");
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream stream(&file);
        QString sStyle = stream.readAll();
        this->setStyleSheet(sStyle);
        file.close();
    }
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onExportParametersToSCAD()
{
    QString sOutputFileName = QFileDialog::getSaveFileName(this, tr("Enter output SCAD file name"), ".", tr("SCAD (*.scad)"));
    if (!sOutputFileName.isEmpty())
        m_pController->exportParametersToSCAD(sOutputFileName);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onExportParametersToTXT()
{
    QString sOutputFileName = QFileDialog::getSaveFileName(this, tr("Select output TXT parameter file"), QCoreApplication::applicationDirPath(), tr("TXT (*.txt)"));
    if (!sOutputFileName.isEmpty())
        m_pController->exportParametersToTXT(sOutputFileName);
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
    ui->tabWidget->setCurrentIndex(CREATE_KEY_TAB);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onUseExistingKeyClicked()
{
    ui->tabWidget->setCurrentIndex(USE_EXISTING_KEY_TAB);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onVisualizeSTLClicked()
{
    ui->tabWidget->setCurrentIndex(VISUALIZE_STL_TAB);
}
