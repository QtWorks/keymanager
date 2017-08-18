#ifndef CONTROLLER_H
#define CONTROLLER_H

// Qt
#include <QObject>
#include <QThread>

// Application
#include "parameter.h"
#include "cxmlnode.h"
#include "iservice.h"
class ParameterMgr;
class WidgetFactory;
class CollapsibleBlock;
class OpenSCADWrapper;

class Controller : public QObject, public IService
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    Controller(QObject *parent=nullptr);

    //! Destructor
    ~Controller();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return menu 1 node
    const CXMLNode &menu1Node() const;

    //! Return menu 2 node
    const CXMLNode &menu2Node() const;

    //! Return menu 3 node
    const CXMLNode &menu3Node() const;

    //! Return settings node
    const CXMLNode &settingsNode() const;

    //! Return parameter manager
    ParameterMgr *parameterMgr() const;

    //! Return widget factory
    WidgetFactory *widgetFactory() const;

    //! Return OpenSCAD wrapper
    OpenSCADWrapper *openSCADwrapper() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Startup
    virtual bool startup();

    //! Shutdown
    virtual void shutdown();

    //! Export parameters to SCAD
    bool exportParametersToSCAD(QString &sOutputFileName);

    //! Export parameters to TXT
    void exportParametersToTXT(const QString &sOutputFileName);

    //! Import parameters from TXT
    void importParametersFromTXT(const QString &sInputFileName);

    //! Generate STL
    void generateSTL();

private:
    //! Load settings
    bool loadSettings();

private:
    //! Parameter manager
    ParameterMgr *m_pParameterMgr;

    //! Widget factory
    WidgetFactory *m_pWidgetFactory;

    //! Parameter thread
    QThread m_tParameterThread;

    //! Open SCAD path
    QString m_sOpenSCADPath;

    //! Open SCAD wrapper
    OpenSCADWrapper *m_pOpenSCADWrapper;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Parameter value changed
    void onParameterValueChanged(const QString &sParameterName, const QString &sParameterValue);

    //! Update widget value
    void onUpdateWidgetValue(const QString &sParameterVariable, const QString &sVariableValue);

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //--------------------------------------

    //! STL file ready
    void STLFileReady(const QString &sPath);

    //! Output SCAD ready
    void outputSCADReady(const QString &sPath);
};

#endif // CONTROLLER_H
