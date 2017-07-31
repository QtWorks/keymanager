// Qt
#include <QDebug>

// Application
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "controller.h"
#include "keyblock.h"
#include "constants.h"
#include "collapsibleblock.h"
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

    // Build menu 1 tab
    buildMenu1Tab(m_pController->menu1Node());

    // Build menu 2 tab
    buildMenu2Tab(m_pController->menu2Node());

    // Collapse/Expand all
    connect(ui->closeAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onExpandAll);
    connect(ui->openAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onCollapseAll);

    // Generate script
    connect(ui->generateScriptButton, &QPushButton::clicked, m_pController, &Controller::onGenerateScript);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::buildMenu1Tab(const CXMLNode &xNode)
{
    QVector<CXMLNode> vBlocks = xNode.getNodesByTagName(TAG_BLOCK);
    ui->menu1LayoutMgr->setSize(vBlocks.size());
    foreach (CXMLNode xKeyBlock, vBlocks)
        buildBlock(xKeyBlock);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::buildBlock(const CXMLNode &xBlock, CollapsibleBlock *pParentBlock)
{
    // Create new key block
    KeyBlock *pKeyBlock = new KeyBlock(xBlock);
    if (pKeyBlock != nullptr)
    {
        // Listen to parameter value changed
        connect(pKeyBlock, &KeyBlock::parameterValueChanged, m_pController, &Controller::onParameterValueChanged);

        // Add block
        if (pParentBlock == nullptr)
        {
            pParentBlock = ui->menu1LayoutMgr->addBlock(pKeyBlock, pKeyBlock->name(), pKeyBlock->hasParameters());
            connect(pParentBlock, &CollapsibleBlock::blockSelected, pKeyBlock, &KeyBlock::onSelectMe);
        }
        else
        {
            pParentBlock->addWidgetInLayout(pKeyBlock);
        }

        QVector<CXMLNode> vChildBlocks = xBlock.getNodesByTagName(TAG_BLOCK);
        foreach (CXMLNode xChildBlock, vChildBlocks)
            buildBlock(xChildBlock, pParentBlock);
    }
}

//-------------------------------------------------------------------------------------------------

void MainWindow::buildMenu2Tab(const CXMLNode &xNode)
{

}

