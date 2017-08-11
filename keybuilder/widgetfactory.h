#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H

// Qt
#include <QObject>
#include <QHash>

// Application
#include "cxmlnode.h"
class Controller;
class BaseWidget;

class WidgetFactory : public QObject
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit WidgetFactory(QObject *parent = nullptr);

    //! Destructor
    ~WidgetFactory();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Set controller
    void setController(Controller *pController);

    //! Return widget by variable name
    BaseWidget *getWidgetByVariableName(const QString &sParameterVariable) const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Build widget
    BaseWidget *buildWidget(const CXMLNode &xNode, QWidget *pParentWidget=nullptr);

private:
    //! Controller
    Controller *m_pController;

    //! Widget hash
    QHash<QString, BaseWidget *> m_hWidgetHash;
};

#endif // WIDGETFACTORY_H
