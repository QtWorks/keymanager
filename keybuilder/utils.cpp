// Windows
#include <stdio.h>
#include <windows.h>

// Qt
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

// Application
#include "utils.h"
#define OUTPUT_DIR "output"

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
    QDir dRootDir = appDir();
    dRootDir.cdUp();
    dRootDir.mkdir(OUTPUT_DIR);
    dRootDir.cd(OUTPUT_DIR);
    return dRootDir;
}
