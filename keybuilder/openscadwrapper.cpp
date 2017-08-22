// Qt
#include <QFileInfo>
#include <QProcess>
#include <QDateTime>
#include <QFileSystemWatcher>
#include <QDebug>

// Application
#include "openscadwrapper.h"
#include "helper.h"
#include "utils.h"
#include "controller.h"
#define TARGET_STRING "OpenSCAD_Model"
#define OUTPUT_STRING "OFC3DKEY"

//-------------------------------------------------------------------------------------------------

OpenSCADWrapper::OpenSCADWrapper(const QString &sOpenSCADPath, QObject *parent) : QObject(parent),
    m_pController(nullptr), m_sOpenSCADPath(sOpenSCADPath), m_pProcess(new QProcess(this)),
    m_pFileSystemWatcher(new QFileSystemWatcher(this)), m_sNextOutputSTLFile("")
{
    // Add path
    m_pFileSystemWatcher->addPath(Utils::outputDir().absolutePath());
    connect(m_pFileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &OpenSCADWrapper::onOutputDirectoryChanged, Qt::UniqueConnection);
    connect(m_pFileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &OpenSCADWrapper::onOutputDirectoryChanged, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

void OpenSCADWrapper::setController(Controller *pController)
{
    m_pController = pController;
    connect(m_pProcess, SIGNAL(finished(int, QProcess::ExitStatus)), m_pController, SLOT(onOpenSCADProcessComplete(int, QProcess::ExitStatus)), Qt::UniqueConnection);
    connect(m_pProcess, &QProcess::readyReadStandardOutput, m_pController, &Controller::onOpenSCADreadyReadStandardOutput, Qt::UniqueConnection);
    connect(m_pProcess, &QProcess::readyReadStandardError, m_pController, &Controller::onOpenSCADreadyReadStandardError, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

OpenSCADWrapper::~OpenSCADWrapper()
{

}

//-------------------------------------------------------------------------------------------------

bool OpenSCADWrapper::generateSTL(const QString &sInputSCAD)
{
    m_sNextOutputSTLFile.clear();
    QFileInfo fi(sInputSCAD);
    if (fi.exists())
    {
        // Compute output filename
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString sOutputFileName = QString("output_stl_%1_%2_%3_%4_%5_%6.stl").arg(currentDateTime.date().day()).
            arg(currentDateTime.date().month()).arg(currentDateTime.date().year()).
                arg(currentDateTime.time().hour()).arg(currentDateTime.time().minute()).arg(currentDateTime.time().second());

        // Compute out STL file name
        m_sNextOutputSTLFile = Utils::outputDir().absoluteFilePath(sOutputFileName);
        QString sProgram = QString("\"%1\" -o \"%2\" \"%3\"").arg(m_sOpenSCADPath).arg(m_sNextOutputSTLFile).arg(sInputSCAD);
        logInfo(sProgram);

        // Setup process
        QStringList lArgs;
        m_pProcess->start(sProgram, lArgs);
        return true;
    }

    QString sMsg = QString("%1 DOES NOT EXIST").arg(sInputSCAD);
    logError(sMsg);
    return false;
}

//-------------------------------------------------------------------------------------------------

void OpenSCADWrapper::onOutputDirectoryChanged(const QString &sPath)
{
    Q_UNUSED(sPath);
    QFileInfo fi(m_sNextOutputSTLFile);
    if (fi.exists())
    {
        Utils::replaceInFile(m_sNextOutputSTLFile, TARGET_STRING, OUTPUT_STRING);
        emit STLFileReady(m_sNextOutputSTLFile);
    }
}
