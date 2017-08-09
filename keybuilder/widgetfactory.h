#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H

// Qt
#include <QObject>

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
    explicit WidgetFactory(QObject *parent = 0);

    //! Destructor
    ~WidgetFactory();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Set controller
    void setController(Controller *pController);

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Build widget
    BaseWidget *buildWidget(const CXMLNode &xNode, QWidget *pParentWidget=nullptr);

private:
    //! Controller
    Controller *m_pController;
};

#endif // WIDGETFACTORY_H
