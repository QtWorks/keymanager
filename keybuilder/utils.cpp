// Windows
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <random>
#include <cmath>

// Qt
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QTime>
#include <QtGlobal>
#include <QCryptographicHash>
#include <QDebug>

// Application
#include "utils.h"
#include "constants.h"
#include "helper.h"

//-------------------------------------------------------------------------------------------------

QString Utils::getDiskSerial()
{
    DWORD dwVolSerial;
    BOOL bIsRetrieved = GetVolumeInformation(L"C:\\", NULL, NULL, &dwVolSerial, NULL, NULL, NULL, NULL);
    if (bIsRetrieved)
        return QString::number(dwVolSerial, 16);
    return QString("");
}

//-------------------------------------------------------------------------------------------------

QString Utils::getDiskSerialHash()
{
    return QCryptographicHash::hash(Utils::getDiskSerial().toLatin1(), QCryptographicHash::Sha256).toHex().toUpper();
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

//-------------------------------------------------------------------------------------------------

QString Utils::randHex(int iLength)
{
    qsrand(QTime::currentTime().msec());
    const QString sPossibleCharacters("0123456789ABCDEF");
    QString sRandomString;
    for(int i=0; i<iLength; ++i)
    {
        int index = qrand() % sPossibleCharacters.length();
        QChar nextChar = sPossibleCharacters.at(index);
        sRandomString.append(nextChar);
    }
    return sRandomString.toUpper();
}

//-------------------------------------------------------------------------------------------------

QString Utils::simpleEncode(const QString &sInput, const QVector<int> &vOrder)
{
    QString sOutput("");
    for (int i=0; i<vOrder.size(); i++)
    {
        if ((vOrder[i] >= 0) && (vOrder[i] < sInput.length()))
            sOutput += sInput.at(vOrder[i]);
    }
    if (sOutput.length() != sInput.length())
    {
        logWarning("FAILED TO REORDER INPUT STRING");
        return sInput;
    }
    return sOutput;
}

//-------------------------------------------------------------------------------------------------

QString Utils::simpleDecode(const QString &sInput, const QVector<int> &vOrder)
{
    QString sOutput("");
    for (int i=0; i<vOrder.size(); i++)
    {
        int iIndex = vOrder.indexOf(i);

        if ((iIndex >= 0) && (iIndex < sInput.length()))
            sOutput += sInput.at(iIndex);
    }
    if (sOutput.length() != sInput.length())
    {
        logWarning("FAILED TO REORDER INPUT STRING");
        return sInput;
    }
    return sOutput;
}

//-------------------------------------------------------------------------------------------------

QVector<int> Utils::buildOrder(int nItems)
{
    qsrand(QTime::currentTime().msec());
    QVector<int> vOrigOrder;
    for (int i=0; i<nItems; i++)
        vOrigOrder << i;
    QVector<int> vOut;
    while (!vOrigOrder.isEmpty())
    {
        int iBottom = 0;
        int iTop = vOrigOrder.size()-1;
        int iRand = qrand()%((iTop + 1)-iBottom)+iBottom;
        vOut << vOrigOrder[iRand];
        vOrigOrder.takeAt(iRand);
    }
    return vOut;
}
