// Qt
#include <QDebug>

// Application
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller.h"
#include "constants.h"
#include <src/stlwindow.h>

//-------------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);

    // Add STL Window
    ui->stlViewerLayout->addWidget(new STLWindow);
}

//-------------------------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void MainWindow::setController(Controller *pController)
{
    m_pController = pController;

    // Build menu 1 tab
    buildMenu1Tab(m_pController->menu1Node());

    // Build menu 2 tab
    buildMenu2Tab(m_pController->menu2Node());

    // Collapse/Expand all
    connect(ui->closeAllButtonMenu1, &QPushButton::clicked, ui->layoutMgr, &LayoutMgr::onExpandAll);
    connect(ui->openAllButtonMenu1, &QPushButton::clicked, ui->layoutMgr, &LayoutMgr::onCollapseAll);

    // Generate script
    connect(ui->generateScriptButton, &QPushButton::clicked, m_pController, &Controller::onGenerateScript);

    // Listen parameter change
    connect(ui->layoutMgr, &LayoutMgr::parameterValueChanged, m_pController, &Controller::onParameterValueChanged);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::buildMenu1Tab(const CXMLNode &xNode)
{
    QVector<CXMLNode> vKeyBlocks = xNode.getNodesByTagName(TAG_KEY);
    foreach (CXMLNode xKeyBlock, vKeyBlocks)
        // Create a new key block
        ui->layoutMgr->addKeyBlock(xKeyBlock);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::buildMenu2Tab(const CXMLNode &xNode)
{

}

