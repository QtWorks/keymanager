// Qt
#include <QDebug>

// Application
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller.h"
#include "parameterblock.h"
#include "constants.h"
#include "collapsibleblock.h"
#include "parametermgr.h"
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
    // Set controller
    m_pController = pController;

    // Listen to parameter value changed
    ui->menu1LayoutMgr->setController(m_pController);
    ui->menu2LayoutMgr->setController(m_pController);

    // Close/Open all
    connect(ui->closeAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onCloseAll);
    connect(ui->openAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onOpenAll);
    connect(ui->closeAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onCloseAll);
    connect(ui->openAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onOpenAll);

    // Build menu 1 tab
    ui->menu1LayoutMgr->buildMenu(m_pController->menu1Node());

    // Build menu 2 tab
    ui->menu2LayoutMgr->buildMenu(m_pController->menu2Node());
}
