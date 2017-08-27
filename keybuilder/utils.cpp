// Windows
#include <stdio.h>
#include <windows.h>

// Qt
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QTime>
#include <QtGlobal>
#include <QDebug>

// Application
#include "utils.h"
#define OUTPUT_DIR "OFC3DKEY_output"
#define TEMPLATES_DIR "templates"
#define OPENSCAD_DIR "STLCompiler"
#define OPENSCAD_EXE "stlcompiler.exe"

//-------------------------------------------------------------------------------------------------

QByteArray Utils::getDiskSerial()
{
    DWORD dwVolSerial;
    BOOL bIsRetrieved = GetVolumeInformation(L"C:\\", NULL, NULL, &dwVolSerial, NULL, NULL, NULL, NULL);
    if (bIsRetrieved)
        return QString::number(dwVolSerial, 16).toLatin1();
    return QByteArray();
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
        qDebug() << sFileContents;
        while (sFileContents.contains(sInputString, iSensitivity))
            sFileContents.replace(sInputString, sOutputString);
        saveFile(sFileContents, sInputFile);
    }
}

//-------------------------------------------------------------------------------------------------

QByteArray Utils::getRandomHex(const int &iLength)
{    
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QString sRandomHex;

    for(int i=0; i<iLength; i++)
    {
        int n = qrand() % 16;
        sRandomHex.append(QString::number(n, 16));
    }

    return sRandomHex.toLatin1();
}
