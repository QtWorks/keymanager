#ifndef CONTROLLER_H
#define CONTROLLER_H

// Qt
#include <QObject>
#include <QProcess>

// Application
#include "parameter.h"
#include "cxmlnode.h"
#include "iservice.h"
class ParameterMgr;
class WidgetFactory;
class CollapsibleBlock;
class OpenSCADWrapper;
class QImage;

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

    //! Debug on?
    bool debugOn() const;

    //! Return key preview image
    QImage *keyPreviewImage() const;

    //! Return next output STL file
    QString nextOutputSTLFile() const;

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

    //! Load key preview image
    void loadKeyPreviewImage(const QString &sKeyImagePreview);

private:
    //! Load settings
    bool loadSettings();

    //! Clear output directory
    void clearOutputDirectory();

private:
    //! Parameter manager
    ParameterMgr *m_pParameterMgr;

    //! Widget factory
    WidgetFactory *m_pWidgetFactory;

    //! Open SCAD path
    QString m_sOpenSCADPath;

    //! Open SCAD wrapper
    OpenSCADWrapper *m_pOpenSCADWrapper;

    //! Debug mode
    bool m_bDebugOn;

    //! Key preview image
    QImage *m_pKeyPreviewImage;

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

    //! STL file error
    void STLFileError(const QString &sPath);

    //! Output SCAD ready
    void outputSCADReady(const QString &sPath);

    //! OpenSCAD process complete
    void openSCADProcessComplete(const QString &sMsg);

    //! OpenSCAD standard output ready
    void openSCADStandardOutputReady(const QString &sMsg);

    //! OpenSCAD standard error ready
    void openSCADStandardErrorReady(const QString &sMsg);

    //! Update key previews
    void updateKeyPreviews();

    //! Block selection status changed
    void blockSelectionStatusChanged(CollapsibleBlock *pBlock);
};

#endif // CONTROLLER_H
