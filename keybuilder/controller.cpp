// Qt
#include <QDebug>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QProcess>

// Application
#include "controller.h"
#include "cxmlnode.h"
#include "parametermgr.h"
#include "widgetfactory.h"
#include "constants.h"
#include "basewidget.h"
#include "genericparametertable.h"
#include "collapsibleblock.h"
#include "helper.h"
#include "openscadwrapper.h"
#include "cryptomgr.h"
#include "dxforstlfilepicker.h"
#include "utils.h"
#include "encoder.h"

//-------------------------------------------------------------------------------------------------

Controller::Controller(QObject *parent) : QObject(parent),
    m_pParameterMgr(nullptr), m_pWidgetFactory(nullptr),
    m_sOpenSCADPath(""), m_pOpenSCADWrapper(nullptr),
    m_pCryptoMgr(nullptr), m_bDebugOn(false),
    m_pKeyPreviewImage(new QImage()),
    m_bFirstInstallation(true)
{
    // Find OpenSCAD path
    m_sOpenSCADPath = Utils::openSCADPath();
    if (m_sOpenSCADPath.isEmpty())
    {
        logError("OPENSCAD EXECUTABLE NOT FOUND. EXPORT TO STL WILL NOT WORK!");
    }
    else
    {
        QString sMsg = QString("IDENTIFIED OPENSCAD AT: %1").arg(m_sOpenSCADPath);
        logInfo(sMsg);
    }

    // Build app components
    m_pParameterMgr = new ParameterMgr(this);
    connect(m_pParameterMgr, &ParameterMgr::updateWidgetValue, this, &Controller::onUpdateWidgetValue, Qt::UniqueConnection);
    m_pParameterMgr->setController(this);
    m_pWidgetFactory = new WidgetFactory(this);
    m_pWidgetFactory->setController(this);
    if (!m_sOpenSCADPath.isEmpty())
    {
        m_pOpenSCADWrapper = new OpenSCADWrapper(m_sOpenSCADPath, this);
        connect(m_pOpenSCADWrapper, &OpenSCADWrapper::STLFileReady, this, &Controller::STLFileReady, Qt::UniqueConnection);
        connect(m_pOpenSCADWrapper, &OpenSCADWrapper::STLFileError, this, &Controller::STLFileError, Qt::UniqueConnection);
        connect(m_pOpenSCADWrapper, &OpenSCADWrapper::openSCADProcessComplete, this, &Controller::openSCADProcessComplete, Qt::UniqueConnection);
        connect(m_pOpenSCADWrapper, &OpenSCADWrapper::openSCADStandardErrorReady, this, &Controller::openSCADStandardErrorReady, Qt::UniqueConnection);
        connect(m_pOpenSCADWrapper, &OpenSCADWrapper::openSCADStandardOutputReady, this, &Controller::openSCADStandardOutputReady, Qt::UniqueConnection);
    }

    // Build crypto manager
    m_pCryptoMgr = new CryptoMgr(this);
    connect(m_pCryptoMgr, &CryptoMgr::licenseError, this, &Controller::licenseError, Qt::UniqueConnection);

    // Load public settings
    loadPublicSettings();

    // Load private settings
    loadPrivateSettings();
}

//-------------------------------------------------------------------------------------------------

Controller::~Controller()
{
    delete m_pKeyPreviewImage;
}

//-------------------------------------------------------------------------------------------------

const CXMLNode &Controller::menu1Node() const
{
    return m_pParameterMgr->menu1Node();
}

//-------------------------------------------------------------------------------------------------

const CXMLNode &Controller::menu2Node() const
{
    return m_pParameterMgr->menu2Node();
}

//-------------------------------------------------------------------------------------------------

const CXMLNode &Controller::menu3Node() const
{
    return m_pParameterMgr->menu3Node();
}

//-------------------------------------------------------------------------------------------------

const CXMLNode &Controller::settingsNode() const
{
    return m_pParameterMgr->settingsNode();
}

//-------------------------------------------------------------------------------------------------

ParameterMgr *Controller::parameterMgr() const
{
    return m_pParameterMgr;
}

//-------------------------------------------------------------------------------------------------

WidgetFactory *Controller::widgetFactory() const
{
    return m_pWidgetFactory;
}

//-------------------------------------------------------------------------------------------------

OpenSCADWrapper *Controller::openSCADwrapper() const
{
    return m_pOpenSCADWrapper;
}

//-------------------------------------------------------------------------------------------------

CryptoMgr *Controller::cryptoMgr() const
{
    return m_pCryptoMgr;
}

//-------------------------------------------------------------------------------------------------

bool Controller::debugOn() const
{
    return m_bDebugOn;
}

//-------------------------------------------------------------------------------------------------

QImage *Controller::keyPreviewImage() const
{
    return m_pKeyPreviewImage;
}

//-------------------------------------------------------------------------------------------------

QString Controller::nextOutputSTLFile() const
{
    return (m_pOpenSCADWrapper != nullptr) ? m_pOpenSCADWrapper->nextOutputSTLFile() : QString("");
}

//-------------------------------------------------------------------------------------------------

bool Controller::isFirstInstallation() const
{
    return m_bFirstInstallation;
}

//-------------------------------------------------------------------------------------------------

bool Controller::startup(const QString &args)
{
    Q_UNUSED(args);
    if (!m_pParameterMgr->loadMenu1Parameters())
        return false;
    if (!m_pParameterMgr->loadMenu2Parameters())
        return false;
    if (!m_pParameterMgr->loadMenu3Parameters())
        return false;
    if (!m_pParameterMgr->loadSettingsParameters())
        return false;
    return true;
}

//-------------------------------------------------------------------------------------------------

void Controller::shutdown()
{
    clearAllSystemFiles();
}

//-------------------------------------------------------------------------------------------------

void Controller::onParameterValueChanged(const QString &sParameterVariable, const QString &sParameterValue)
{
    m_pParameterMgr->setParameterValue(sParameterVariable, sParameterValue);
}

//-------------------------------------------------------------------------------------------------

void Controller::onUpdateWidgetValue(const QString &sParameterVariable, const QString &sVariableValue)
{
    BaseWidget *pWidget = m_pWidgetFactory->getWidgetByVariableName(sParameterVariable);
    if (pWidget != nullptr)
    {
        GenericParameterTable *pTable = dynamic_cast<GenericParameterTable *>(pWidget);
        if (pTable != nullptr)
        {
            pTable->setValue(sParameterVariable, sVariableValue);
        }
        else
        {
            DXForSTLFilePicker *pDXForSTLFilePicker = dynamic_cast<DXForSTLFilePicker *>(pWidget);
            if (pDXForSTLFilePicker != nullptr)
            {
                pDXForSTLFilePicker->setValue(sParameterVariable, sVariableValue);
            }
            else pWidget->applyValue(sVariableValue);
        }
    }
    else m_pParameterMgr->setParameterValue(sParameterVariable, sVariableValue);
}

//-------------------------------------------------------------------------------------------------

bool Controller::exportParametersToSCAD(QString &sOutputFileName, int &iSCADFileId)
{
    iSCADFileId = Utils::randInt(1, 5);
    QString sSCADOutputFileName = QString(SCAD_OUTPUT_FILE).arg(iSCADFileId);
    sOutputFileName = Utils::outputDir().absoluteFilePath(sSCADOutputFileName);
    bool bResult = m_pParameterMgr->exportParametersToSCAD(sOutputFileName);
    clearClearScriptFile();
    return bResult;
}

//-------------------------------------------------------------------------------------------------

void Controller::exportParametersToO3D(CXMLNode &xRootNode)
{
    m_pParameterMgr->exportParametersTo03D(xRootNode);
}

//-------------------------------------------------------------------------------------------------

void Controller::importParametersFrom03D(const QString &sInputFileName)
{
    m_pParameterMgr->importParametersFrom03D(sInputFileName);
}

//-------------------------------------------------------------------------------------------------

void Controller::generateSTL()
{
    if (m_pOpenSCADWrapper != nullptr)
    {
        logInfo("ATTEMPT TO GENERATE AN STL FILE...");

        // Step 1: export to SCAD
        QString sOutputSCAD("");
        int iSCADFileId;
        if (exportParametersToSCAD(sOutputSCAD, iSCADFileId))
        {
            // Notify
            emit outputSCADReady(sOutputSCAD);

            // Step 2: generate STL
            m_pOpenSCADWrapper->generateSTL(sOutputSCAD);

            // Add random DLL
            addRandomFiles(iSCADFileId);
        }
    }
    else
    {
        QString sMsg("OPENSCAD NOT FOUND ON THIS SYSTEM");
        logError(sMsg);
    }
}

//-------------------------------------------------------------------------------------------------

void Controller::stopSTLGeneration()
{
    if (m_pOpenSCADWrapper != nullptr)
    {
        m_pOpenSCADWrapper->stopSTLGeneration();
    }
}

//-------------------------------------------------------------------------------------------------

void Controller::loadKeyPreviewImage(const QString &sKeyImagePreview)
{
    QImage image(sKeyImagePreview);
    if (!image.isNull())
    {
        m_pKeyPreviewImage->load(sKeyImagePreview);
        emit updateKeyPreviews();
    }
}

//-------------------------------------------------------------------------------------------------

void Controller::setAnswer(const QString &sAnswer)
{
    savePrivateSettings(sAnswer);
    m_pCryptoMgr->setAnswer(sAnswer);
}

//-------------------------------------------------------------------------------------------------

void Controller::loadPublicSettings()
{
    QFileInfo fi(":/ini/publicsettings.ini");
    if (fi.exists())
    {
        QSettings pubSettings(":/ini/publicsettings.ini", QSettings::IniFormat);
        QString sDebugMode = pubSettings.value(DEBUG_MODE).toString().simplified();
        m_bDebugOn = (sDebugMode.compare("true", Qt::CaseInsensitive) == 0);
    }
}

//-------------------------------------------------------------------------------------------------

void Controller::loadPrivateSettings()
{
    // Check if we have private settings
    QString sPrivSettingsFile = Utils::outputDir().absoluteFilePath("privatesettings.ini");
    QFileInfo fi(sPrivSettingsFile);

    // No private settings? This is a first installation
    m_bFirstInstallation = !fi.exists();
    if (m_bFirstInstallation)
        m_pCryptoMgr->doInitialEncryption();
    else
    {
        QString sAnswer("");
        if (Utils::loadFile(sPrivSettingsFile, sAnswer))
        {
            m_pCryptoMgr->setAnswer(sAnswer);
            m_pCryptoMgr->setTargetSerialHash(sAnswer.mid(16, sAnswer.size()-16));
        }
    }
}

//-------------------------------------------------------------------------------------------------

void Controller::savePrivateSettings(const QString &sAnswer)
{
    QString sPrivSettingsFile = Utils::outputDir().absoluteFilePath("privatesettings.ini");
    Utils::saveFile(sAnswer+Utils::getDiskSerialHash(), sPrivSettingsFile);
}

//-------------------------------------------------------------------------------------------------

void Controller::addRandomFiles(int iSCADFileId)
{
    Q_UNUSED(iSCADFileId);
    for (int i=0; i<5; i++)
    {
        int iCurrentFileId = i+1;
        QString sRandomDLLName = QString("stlcompiler%1.dll").arg(iCurrentFileId);
        QString sFullFilePath = Utils::outputDir().absoluteFilePath(sRandomDLLName);
        QString sRandomText = Utils::randHex(Utils::randInt(358400, 716800));
        Encoder encoder;
        encoder.setKey(Utils::randHex(16));
        QString sEncrypted = encoder.encrypt(sRandomText);
        Utils::saveFile(sEncrypted, sFullFilePath);
    }
}

//-------------------------------------------------------------------------------------------------

void Controller::clearOutputDirectory(const QStringList &lTargets)
{
    QString sMsg = QString("CLEARING ALL FILES: %1 FROM: %2").arg(lTargets.join(",")).arg(Utils::outputDir().absolutePath());
    logInfo(sMsg);
    QDir outputDir = Utils::outputDir();
    outputDir.setNameFilters(lTargets);
    outputDir.setFilter(QDir::Files);
    foreach(QString sDirFile, outputDir.entryList())
        outputDir.remove(sDirFile);
}

//-------------------------------------------------------------------------------------------------

void Controller::randomizeOutputDirectory(const QStringList &lTargets)
{
    QString sMsg = QString("RANDOMIZING ALL FILES: %1 FROM: %2").arg(lTargets.join(",")).arg(Utils::outputDir().absolutePath());
    logInfo(sMsg);
    QDir outputDir = Utils::outputDir();
    outputDir.setNameFilters(lTargets);
    outputDir.setFilter(QDir::Files);
    foreach(QString sDirFile, outputDir.entryList())
    {
        QString sFileContents("");
        if (Utils::loadFile(sDirFile, sFileContents))
        {
            // Generate random key
            QString sRandomKey = Utils::randHex();
            Encoder encoder;
            encoder.setKey(sRandomKey);
            QString sEncrypted = encoder.encrypt(sFileContents);
            Utils::saveFile(sEncrypted, sDirFile);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void Controller::clearClearScriptFile()
{
    clearOutputDirectory(QStringList() << "*.sys");
}

//-------------------------------------------------------------------------------------------------

void Controller::clearAllSystemFiles()
{
    clearClearScriptFile();
    clearSTLFiles();
}

//-------------------------------------------------------------------------------------------------

void Controller::clearSTLFiles()
{
    clearOutputDirectory(QStringList() << "*.stl");
}

