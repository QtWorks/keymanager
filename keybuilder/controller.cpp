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
#include "utils.h"
#define DEBUG_MODE "DEBUG"
#define SCAD_OUTPUT_FILE "script_out.scad"

//-------------------------------------------------------------------------------------------------

Controller::Controller(QObject *parent) : QObject(parent),
    m_sOpenSCADPath(""), m_pOpenSCADWrapper(nullptr), m_bDebugOn(true),
    m_pKeyPreviewImage(new QImage())
{
    // Load settings
    loadSettings();

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
        m_pOpenSCADWrapper->setController(this);
        connect(m_pOpenSCADWrapper, &OpenSCADWrapper::STLFileReady, this, &Controller::STLFileReady, Qt::UniqueConnection);
        connect(m_pOpenSCADWrapper, &OpenSCADWrapper::STLFileError, this, &Controller::STLFileError, Qt::UniqueConnection);
        connect(m_pOpenSCADWrapper, &OpenSCADWrapper::openSCADProcessComplete, this, &Controller::openSCADProcessComplete, Qt::UniqueConnection);
        connect(m_pOpenSCADWrapper, &OpenSCADWrapper::openSCADStandardErrorReady, this, &Controller::openSCADStandardErrorReady, Qt::UniqueConnection);
        connect(m_pOpenSCADWrapper, &OpenSCADWrapper::openSCADStandardOutputReady, this, &Controller::openSCADStandardOutputReady, Qt::UniqueConnection);
    }
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

bool Controller::startup()
{
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
    clearOutputDirectory();
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
        else pWidget->applyValue(sVariableValue);
    }
    else m_pParameterMgr->setParameterValue(sParameterVariable, sVariableValue);
}

//-------------------------------------------------------------------------------------------------

bool Controller::exportParametersToSCAD(QString &sOutputFileName)
{
    sOutputFileName = Utils::outputDir().absoluteFilePath(SCAD_OUTPUT_FILE);
    return m_pParameterMgr->exportParametersToSCAD(sOutputFileName);
}

//-------------------------------------------------------------------------------------------------

void Controller::exportParametersToTXT(const QString &sOutputFileName)
{
    m_pParameterMgr->exportParametersToTXT(sOutputFileName);
}

//-------------------------------------------------------------------------------------------------

void Controller::importParametersFromTXT(const QString &sInputFileName)
{
    m_pParameterMgr->importParametersFromTXT(sInputFileName);
}

//-------------------------------------------------------------------------------------------------

void Controller::generateSTL()
{
    if (m_pOpenSCADWrapper != nullptr)
    {
        logInfo("ATTEMPT TO GENERATE AN STL FILE...");

        // Step 1: export to SCAD
        QString sOutputSCAD("");
        if (exportParametersToSCAD(sOutputSCAD))
        {
            // Notify
            emit outputSCADReady(sOutputSCAD);

            // Step 2: generate STL
            m_pOpenSCADWrapper->generateSTL(sOutputSCAD);
        }
    }
    else
    {
        QString sMsg = QString("OPENSCAD NOT FOUND ON THIS SYSTEM");
        logError(sMsg);
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

bool Controller::loadSettings()
{
    QFileInfo fi(":/ini/settings.ini");
    if (fi.exists())
    {
        QSettings settings(":/ini/settings.ini", QSettings::IniFormat);
        QString sDebugMode = settings.value(DEBUG_MODE).toString().simplified();
        m_bDebugOn = (sDebugMode.compare("true", Qt::CaseInsensitive) == 0);
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------------------------------

void Controller::clearOutputDirectory()
{
    QString sMsg = QString("REMOVING ALL *.scad, *.stl FROM: %1").arg(Utils::outputDir().absolutePath());
    logInfo(sMsg);
    QDir outputDir = Utils::outputDir();
    outputDir.setNameFilters(QStringList() << "*.scad" << "*.stl");
    outputDir.setFilter(QDir::Files);
    foreach(QString sDirFile, outputDir.entryList())
        outputDir.remove(sDirFile);
}
