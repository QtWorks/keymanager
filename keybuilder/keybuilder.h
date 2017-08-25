#ifndef KEYBUILDER_H
#define KEYBUILDER_H

// Qt
#include <QObject>

// Application
#include "iservice.h"
#include "mainwindow.h"
class Controller;

class KeyBuilder : public QObject, public IService
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Destructor
    virtual ~KeyBuilder();

    //! Return instance
    static KeyBuilder *instance();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Start server
    virtual bool startup();

    //! Stop server
    virtual void shutdown();

private:
    //! Constructor
    KeyBuilder(QObject *parent=nullptr);

private:
    //! Controller
    Controller *m_pController;

    //! Main window
    MainWindow m_wMainWindow;

    //! KeyBuilder instance
    static KeyBuilder *sInstance;
};

#endif // KEYBUILDER_H
