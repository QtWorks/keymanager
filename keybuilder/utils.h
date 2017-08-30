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

    //! Load file
    static QString loadFile(const QString &sInputFile);

    //! Save file
    static bool saveFile(const QString &sFileContents, const QString &sOutputFile);

    //! Return application directory
    static QDir appDir();

    //! Return output directory
    static QDir outputDir();

    //! Return data directory
    static QDir templatesDirectory();

    //! Return OpenSCAD path
    static QString openSCADPath();

    //! Replace inputString by output string in given file
    static void replaceInFile(const QString &sInputFile, const QString &sInputString, const QString &sOutputString, Qt::CaseSensitivity iSensitivity=Qt::CaseInsensitive);

    //! Generate random hex
    static QString randHex(int iLength=16);
};

#endif // UTILS_H
