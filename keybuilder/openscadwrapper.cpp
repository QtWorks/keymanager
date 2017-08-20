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

//-------------------------------------------------------------------------------------------------

OpenSCADWrapper::OpenSCADWrapper(const QString &sOpenSCADPath, QObject *parent) : QObject(parent),
    m_sOpenSCADPath(sOpenSCADPath), m_pProcess(new QProcess(this)),
    m_pFileSystemWatcher(new QFileSystemWatcher(this)), m_sNextOutputSTLFile("")
{
    // Add path
    m_pFileSystemWatcher->addPath(Utils::outputDir().absolutePath());
    connect(m_pFileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &OpenSCADWrapper::onOutputDirectoryChanged);
    connect(m_pFileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &OpenSCADWrapper::onOutputDirectoryChanged);

    connect(m_pProcess, &QProcess::readyReadStandardOutput, this, &OpenSCADWrapper::readyReadStandardOutput);
    connect(m_pProcess, &QProcess::readyReadStandardError, this, &OpenSCADWrapper::readyReadStandardError);
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
    QFileInfo fi(m_sNextOutputSTLFile);
    if (fi.exists())
        emit STLFileReady(m_sNextOutputSTLFile);
}
