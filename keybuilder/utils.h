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
};

#endif // UTILS_H
