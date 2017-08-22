// Windows
#include <stdio.h>
#include <windows.h>

// Qt
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QStandardPaths>

// Application
#include "utils.h"
#define OUTPUT_DIR "OFC3DKEY_output"
#define TEMPLATES_DIR "templates"
#define OPENSCAD_DIR "OpenSCAD"
#define OPENSCAD_EXE "openscad.exe"

//-------------------------------------------------------------------------------------------------

QString Utils::getDiskSerial()
{
    DWORD dwVolSerial;
    BOOL bIsRetrieved = GetVolumeInformation(L"C:\\", NULL, NULL, &dwVolSerial, NULL, NULL, NULL, NULL);
    if (bIsRetrieved)
        return QString::number(dwVolSerial);
    return QString("");
}

//-------------------------------------------------------------------------------------------------

QString Utils::loadFile(const QString &sInputFile)
{
    QString sFileContents("");
    QFile file(sInputFile);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream stream(&file);
        sFileContents = stream.readAll();
        file.close();
    }
    return sFileContents;
}

//-------------------------------------------------------------------------------------------------

bool Utils::saveFile(const QString &sFileContents, const QString &sOutputFile)
{
    QFile outputFile(sOutputFile);
    if (outputFile.open(QIODevice::WriteOnly))
    {
        QTextStream outStream(&outputFile);
        outStream << sFileContents;
        outputFile.close();
        return true;
    }
    return false;
}

//-------------------------------------------------------------------------------------------------

QDir Utils::appDir()
{
    return QCoreApplication::applicationDirPath();
}

//-------------------------------------------------------------------------------------------------

QDir Utils::outputDir()
{
    QDir tmpLocation = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    tmpLocation.mkdir(OUTPUT_DIR);
    tmpLocation.cd(OUTPUT_DIR);
    return tmpLocation;
}

//-------------------------------------------------------------------------------------------------

QDir Utils::templatesDirectory()
{
    QDir dataDir = appDir();
    dataDir.cd(TEMPLATES_DIR);
    return dataDir;
}

//-------------------------------------------------------------------------------------------------

QString Utils::openSCADPath()
{
    QDir openSCADDir = appDir();
    openSCADDir.cd(OPENSCAD_DIR);
    QString sOpenSCADPath = openSCADDir.absoluteFilePath(OPENSCAD_EXE);
    QFileInfo fi(sOpenSCADPath);
    if (!fi.exists())
        return QString("");
    return sOpenSCADPath;
}

//-------------------------------------------------------------------------------------------------

void Utils::replaceInFile(const QString &sInputFile, const QString &sInputString, const QString &sOutputString, Qt::CaseSensitivity iSensitivity)
{
    QFileInfo fi(sInputFile);
    if (fi.exists())
    {
        QString sFileContents = Utils::loadFile(sInputFile);
        while (sFileContents.contains(sInputString, iSensitivity))
            sFileContents.replace(sInputString, sOutputString);
        saveFile(sFileContents, sInputFile);
    }
}
