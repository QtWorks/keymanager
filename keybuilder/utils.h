#ifndef UTILS_H
#define UTILS_H

// Qt
#include <QString>
#include <QDir>

class Utils
{
public:
    //! Return disk serial number
    static QString getDiskSerial();

    //! Return disk hash
    static QString getDiskSerialHash();

    //! Load file
    static bool loadFile(const QString &sInputFile, QString &sFileContents);

    //! Save file
    static bool saveFile(const QString &sFileContents, const QString &sOutputFile);

    //! Return application directory
    static QDir appDir();

    //! Return output directory
    static QDir outputDir();

    //! Return data dir
    static QDir dataDir();

    //! Return data directory
    static QDir templatesDirectory();

    //! Return OpenSCAD path
    static QString openSCADPath();

    //! Replace inputString by output string in given file
    static void replaceInFile(const QString &sInputFile, const QString &sInputString, const QString &sOutputString, Qt::CaseSensitivity iSensitivity=Qt::CaseInsensitive);

    //! Generate random hex
    static QString randHex(int iLength=16);

    //! Simple encode
    static QString simpleEncode(const QString &sInput, const QVector<int> &vOrder);

    //! Simple decode
    static QString simpleDecode(const QString &sInput, const QVector<int> &vOrder);

    //! Reorder input array randomly
    static QVector<int> buildOrder(int nItems);

    //! Generate rand int
    static int randInt(int iMin=0, int iMax=256);

    //! Update settings
    static void updateSettings(const QString &sFileName);

    //! Return settings value
    static QString settingsValue(const QString &sName);
};

#endif // UTILS_H
