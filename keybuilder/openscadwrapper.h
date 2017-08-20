#ifndef OPENSCADWRAPPER_H
#define OPENSCADWRAPPER_H

// Qt
#include <QObject>
class QProcess;
class QFileSystemWatcher;

class OpenSCADWrapper : public QObject
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    OpenSCADWrapper(const QString &sOpenSCADPath, QObject *parent=nullptr);

    //! Destructor
    ~OpenSCADWrapper();

    //! Generate STL
    bool generateSTL(const QString &sInputSCAD);

private:
    //! OpenSCAD path
    QString m_sOpenSCADPath;

    //! Own process
    QProcess *m_pProcess;

    //! File system watcher
    QFileSystemWatcher *m_pFileSystemWatcher;

    //! Next output STL file
    QString m_sNextOutputSTLFile;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Output directory changed
    void onOutputDirectoryChanged(const QString &sPath);

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! STL file ready
    void STLFileReady(const QString &sPath);

    //! Get output log
    void readyReadStandardOutput();
    void readyReadStandardError();
};

#endif // OPENSCADWRAPPER_H
