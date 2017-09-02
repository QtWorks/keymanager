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
    ui->menu4LayoutMgr->setController(m_pController);

    connect(ui->menu1LayoutMgr, &LayoutMgr::treeVisibilityChanged, this, &MainWindow::onMenu1TreeVisibilityChanged);
    connect(ui->menu2LayoutMgr, &LayoutMgr::treeVisibilityChanged, this, &MainWindow::onMenu2TreeVisibilityChanged);
    connect(ui->menu3LayoutMgr, &LayoutMgr::treeVisibilityChanged, this, &MainWindow::onMenu3TreeVisibilityChanged);
    connect(ui->menu4LayoutMgr, &LayoutMgr::treeVisibilityChanged, this, &MainWindow::onMenu4TreeVisibilityChanged);

    // Connect menu 1 buttons
    connect(ui->hideTreeButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onShowHideTree, Qt::UniqueConnection);
    connect(ui->closeAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu1, &QPushButton::clicked, ui->menu1LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);
    connect(ui->generateSTLButtonMenu1, &QPushButton::clicked, this, &MainWindow::onGenerateSTL, Qt::UniqueConnection);
    connect(ui->saveKeyParametersButtonMenu1, &QPushButton::clicked, this, &MainWindow::onSaveKeyParameters, Qt::UniqueConnection);

    // Connect menu 2 buttons
    connect(ui->hideTreeButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onShowHideTree, Qt::UniqueConnection);
    connect(ui->closeAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu2, &QPushButton::clicked, ui->menu2LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);
    connect(ui->generateSTLButtonMenu2, &QPushButton::clicked, this, &MainWindow::onGenerateSTL, Qt::UniqueConnection);
    connect(ui->saveKeyParametersButtonMenu2, &QPushButton::clicked, this, &MainWindow::onSaveKeyParameters, Qt::UniqueConnection);

    // Connect menu 3 buttons
    connect(ui->hideTreeButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onShowHideTree, Qt::UniqueConnection);
    connect(ui->closeAllButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onCloseAll, Qt::UniqueConnection);
    connect(ui->openAllButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onOpenAll, Qt::UniqueConnection);
    connect(ui->clearAllButtonMenu3, &QPushButton::clicked, ui->menu3LayoutMgr, &LayoutMgr::onClearAll, Qt::UniqueConnection);
    connect(ui->generateSTLButtonMenu3, &QPushButton::clicked, this, &MainWindow::onGenerateSTL, Qt::UniqueConnection);
    connect(ui->saveKeyParametersButtonMenu3, &QPushButton::clicked, this, &MainWindow::onSaveKeyParameters, Qt::UniqueConnection);

    // Connect menu 4 buttons
    connect(ui->hideTreeButtonMenu4, &QPushButton::clicked, ui->menu4LayoutMgr, &LayoutMgr::onShowHideTree, Qt::UniqueConnection);
    connect(ui->generateSTLButtonMenu4, &QPushButton::clicked, this, &MainWindow::onGenerateSTL, Qt::UniqueConnection);
    connect(ui->saveKeyParametersButtonMenu4, &QPushButton::clicked, this, &MainWindow::onSaveKeyParameters, Qt::UniqueConnection);

    // Connect license widget
    connect(ui->licenseWidget, &LicenseWidget::validateClicked, this, &MainWindow::onValidateLicenseClicked);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::buildMenus()
{
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
    ui->menu4LayoutMgr->buildMenu(m_pController->settingsNode());

    foreach (CollapsibleBlock *pBlock, ui->menu1LayoutMgr->blocks())
        m_hAllBlocks[pBlock->uid()] = pBlock;
    foreach (CollapsibleBlock *pBlock, ui->menu2LayoutMgr->blocks())
        m_hAllBlocks[pBlock->uid()] = pBlock;
    foreach (CollapsibleBlock *pBlock, ui->menu3LayoutMgr->blocks())
        m_hAllBlocks[pBlock->uid()] = pBlock;
    foreach (CollapsibleBlock *pBlock, ui->menu4LayoutMgr->blocks())
        m_hAllBlocks[pBlock->uid()] = pBlock;
}

//-------------------------------------------------------------------------------------------------

void MainWindow::loadCSS()
{
    QString sStyle = Utils::loadFile(":/css/main.css");
    this->setStyleSheet(sStyle);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::setLicenseMode(bool bLicenseMode)
{
    ui->licenseWidget->setVisible(bLicenseMode);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::showApplicationBody(bool bShowApplicationBody)
{
    ui->tabWidget->setVisible(bShowApplicationBody);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::showDebugTab(bool bShowDebugTab)
{
    if (!bShowDebugTab)
        ui->tabWidget->removeTab(OUTPUT_SCAD_TAB);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::showOpenSCADOutputLog(bool bShowOpenSCADOoutputLog)
{
    ui->openSCADOutputLog->setVisible(bShowOpenSCADOoutputLog);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::setQuestion(const QString &sQuestion)
{
    ui->licenseWidget->setQuestion(sQuestion);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onCreateKeyClicked()
{
    ui->tabWidget->setCurrentIndex(MENU1_TAB);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onUseExistingKeyClicked()
{
    QString sInputFileName = QFileDialog::getOpenFileName(this, tr("Select input 03D parameter file"), QCoreApplication::applicationDirPath(), tr("03D (*.o3d)"));
    if (!sInputFileName.isEmpty())
    {
        m_pController->importParametersFrom03D(sInputFileName);
        importBlockParametersFromXML(sInputFileName);
    }
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onVisualizeSTLClicked()
{
    ui->tabWidget->setCurrentIndex(VISUALIZE_STL_TAB);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onGenerateSTL()
{
    ui->openSCADOutputLog->clear();

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
    QString sOutputFileName = QFileDialog::getSaveFileName(this, tr("Select output key parameters file"), QCoreApplication::applicationDirPath(), tr("03D (*.o3d)"));
    if (!sOutputFileName.isEmpty())
    {
        CXMLNode xRootNode(TAG_ROOT);

        // Export parameters
        m_pController->exportParametersToO3D(xRootNode);

        // Export blocks
        exportBlocksToXML(xRootNode);

        // Save
        if (xRootNode.save(sOutputFileName))
        {
            QString sMsg = QString("KEY PARAMETERS SAVED TO: %1").arg(sOutputFileName);
            logInfo(sMsg);
        }
        else
        {
            QString sMsg = QString("COULD NOT SAVE KEY PARAMETERS TO: %1").arg(sOutputFileName);
            logError(sMsg);
        }
    }
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
    if (!sOutputSCADContents.isEmpty())
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
    ui->openSCADOutputLog->append(sStatus);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onOpenSCADStandardErrorReady(const QString &sStatus)
{
    ui->progressBar->setVisible(false);
    QString sMsg = "STL BUILD FAILURE";
    logInfo(sMsg);
    statusBar()->showMessage("STL BUILD FAILURE");
    ui->openSCADOutputLog->append(sStatus);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onOpenSCADStandardOutputReady(const QString &sStatus)
{
    ui->progressBar->setVisible(false);
    statusBar()->showMessage("");
    ui->openSCADOutputLog->append(sStatus);
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onMenu1TreeVisibilityChanged(bool bTreeVisible)
{
    ui->hideTreeButtonMenu1->setText(bTreeVisible ? tr("Hide Tree") : tr("Show Tree"));
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onMenu2TreeVisibilityChanged(bool bTreeVisible)
{
    ui->hideTreeButtonMenu2->setText(bTreeVisible ? tr("Hide Tree") : tr("Show Tree"));
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onMenu3TreeVisibilityChanged(bool bTreeVisible)
{
    ui->hideTreeButtonMenu3->setText(bTreeVisible ? tr("Hide Tree") : tr("Show Tree"));
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onMenu4TreeVisibilityChanged(bool bTreeVisible)
{
    ui->hideTreeButtonMenu4->setText(bTreeVisible ? tr("Hide Tree") : tr("Show Tree"));
}

//-------------------------------------------------------------------------------------------------

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool bCancelled = false;
    if (m_bAppIsDirty)
    {
        QMessageBox msgBox;

        // Save generated STL
        msgBox.setText("Save generated STL?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::No);
        int iValue = msgBox.exec();
        if (iValue == QMessageBox::Yes)
            onSaveGeneratedSTL();
        else
            if (iValue == QMessageBox::Cancel)
                bCancelled = true;

        if (!bCancelled)
        {
            // Save key parameters
            msgBox.setText("Save Key Parameters?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::No);
            iValue = msgBox.exec();
            if (iValue == QMessageBox::Yes)
            {
                onSaveKeyParameters();
                m_bAppIsDirty = false;
            }
            else
                if (iValue == QMessageBox::Cancel)
                    bCancelled = true;
        }
    }
    if (bCancelled)
        event->ignore();
    else
        event->accept();
}

//-------------------------------------------------------------------------------------------------

void MainWindow::exportBlocksToXML(CXMLNode &xRootNode)
{
    QVector<LayoutMgr *> vLayoutMgrs;
    vLayoutMgrs << ui->menu1LayoutMgr << ui->menu2LayoutMgr << ui->menu3LayoutMgr << ui->menu4LayoutMgr;
    CXMLNode xBlocksNode(TAG_BLOCKS);
    foreach (LayoutMgr *pLayoutMgr, vLayoutMgrs)
    {
        QList<CollapsibleBlock *> lBlocks = pLayoutMgr->blocks();
        foreach (CollapsibleBlock *pBlock, lBlocks)
        {
            QString sBlockUID = pBlock->uid();
            bool bBlockIsSelected = pBlock->isSelected();
            bool bBlockIsClosed = pBlock->isClosed();
            CXMLNode xBlockNode(TAG_BLOCK);
            xBlockNode.attributes()[BLOCK_UID] = sBlockUID;
            xBlockNode.attributes()[BLOCK_SELECTED] = QString::number(bBlockIsSelected);
            xBlockNode.attributes()[BLOCK_CLOSED] = QString::number(bBlockIsClosed);
            xBlocksNode.nodes() << xBlockNode;
        }
    }
    xRootNode.nodes() << xBlocksNode;
}

//-------------------------------------------------------------------------------------------------

void MainWindow::importBlockParametersFromXML(const QString &sInputFile)
{
    CXMLNode xRootNode = CXMLNode::loadXMLFromFile(sInputFile, true);
    CXMLNode xBlocks = xRootNode.getNodeByTagName(TAG_BLOCKS);
    QVector<CXMLNode> vChildBlocks = xBlocks.getNodesByTagName(TAG_BLOCK);
    foreach (CXMLNode xChildBlock, vChildBlocks)
    {
        QString sBlockUID = xChildBlock.attributes()[BLOCK_UID];
        QString sBlockSelected = xChildBlock.attributes()[BLOCK_SELECTED];
        bool bBlockSelected = (sBlockSelected == "1");
        QString sBlockClosed = xChildBlock.attributes()[BLOCK_CLOSED];
        bool bBlockClosed = (sBlockClosed == "1");

        CollapsibleBlock *pTargetBlock = m_hAllBlocks[sBlockUID];
        if (pTargetBlock != nullptr)
        {
            pTargetBlock->select(bBlockSelected);
            pTargetBlock->onOpenOrClose(bBlockClosed, false);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void MainWindow::onValidateLicenseClicked(const QString &sQuestion, const QString &sAnswer)
{
    if (m_pController->validateLicense(sQuestion, sAnswer))
    {
        // Start controller
        if (m_pController->startup())
        {
            setLicenseMode(false);
            showApplicationBody(true);
            showOpenSCADOutputLog(m_pController->debugOn());
            showDebugTab(m_pController->debugOn());
            buildMenus();
        }
        else logError("COULD NOT STARTUP CONTROLLER. EXITING.");
    }
}
