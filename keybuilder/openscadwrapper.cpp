// Qt
#include <QFileInfo>
#include <QProcess>
#include <QDateTime>
#include <QDebug>

// Application
#include "openscadwrapper.h"
#include "helper.h"
#include "utils.h"
#include "controller.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

OpenSCADWrapper::OpenSCADWrapper(const QString &sOpenSCADPath, QObject *parent) : QObject(parent),
    m_pController(nullptr), m_sOpenSCADPath(sOpenSCADPath), m_pProcess(new QProcess(this)),
    m_sNextOutputSTLFile("")
{

}

//-------------------------------------------------------------------------------------------------

void OpenSCADWrapper::setController(Controller *pController)
{
    m_pController = pController;
    connect(m_pProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onOpenSCADProcessComplete(int, QProcess::ExitStatus)), Qt::UniqueConnection);
    connect(m_pProcess, &QProcess::readyReadStandardOutput, this, &OpenSCADWrapper::onOpenSCADreadyReadStandardOutput, Qt::UniqueConnection);
    connect(m_pProcess, &QProcess::readyReadStandardError, this, &OpenSCADWrapper::onOpenSCADreadyReadStandardError, Qt::UniqueConnection);
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

const QString &OpenSCADWrapper::nextOutputSTLFile() const
{
    return m_sNextOutputSTLFile;
}

//-------------------------------------------------------------------------------------------------

void OpenSCADWrapper::onOpenSCADProcessComplete(int iExitCode, QProcess::ExitStatus exitStatus)
{
    QString sMsg = QString("OPENSCAD PROCESS TERMINATED WITH CODE: %1 AND EXIT STATUS: %2").arg(iExitCode).arg(exitStatus);
    emit openSCADProcessComplete(sMsg);

    // Process completed, check if file exists
    QFileInfo fi(m_sNextOutputSTLFile);
    if (fi.exists())
    {
        QString sMsg = QString("STL FILE SUCCESSFULLY GENERATED AT: %1").arg(m_sNextOutputSTLFile);
        logInfo(sMsg);
        Utils::replaceInFile(m_sNextOutputSTLFile, TARGET_STRING, OUTPUT_STRING);
        emit STLFileReady(m_sNextOutputSTLFile);
    }
    else
    {
        QString sMsg = QString("COULD NOT GENERATE STL FILE: %1").arg(m_sNextOutputSTLFile);
        emit STLFileError(sMsg);
    }
}

//-------------------------------------------------------------------------------------------------

void OpenSCADWrapper::onOpenSCADreadyReadStandardOutput()
{
    QProcess *pSender = dynamic_cast<QProcess *>(sender());
    if (pSender != nullptr)
    {
        QByteArray bBuffer = pSender->readAllStandardOutput();
        QString sMsg = QString("OPENSCAD OUTPUT: %1").arg(QString(bBuffer));
        emit openSCADStandardOutputReady(sMsg);
    }
}

//-------------------------------------------------------------------------------------------------

void OpenSCADWrapper::onOpenSCADreadyReadStandardError()
{
    QProcess *pSender = dynamic_cast<QProcess *>(sender());
    if (pSender != nullptr)
    {
        QByteArray bBuffer = pSender->readAllStandardError();
        QString sMsg = QString("OPENSCAD ERROR: %1").arg(QString(bBuffer));
        emit openSCADStandardErrorReady(sMsg);
    }
}
