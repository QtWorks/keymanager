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

    //! Constructor
    KeyBuilder(QObject *parent=nullptr);

    //! Destructor
    virtual ~KeyBuilder();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Start server
    virtual bool startup();

    //! Stop server
    virtual void shutdown();

private:
    //! Controller
    Controller *m_pController;

    //! Main window
    MainWindow m_wMainWindow;
};

#endif // KEYBUILDER_H
