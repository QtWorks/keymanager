// Qt
#include <QDebug>

// Application
#include "controller.h"
#include "cxmlnode.h"
#include "parametermgr.h"
#include "widgetfactory.h"
#include "selectionmgr.h"
#include "constants.h"
#include "basewidget.h"

//-------------------------------------------------------------------------------------------------

Controller::Controller(QObject *parent) : QObject(parent)
{
    m_pParameterMgr = new ParameterMgr(this);
    connect(m_pParameterMgr, &ParameterMgr::updateWidgetValue, this, &Controller::onUpdateWidgetValue, Qt::UniqueConnection);
    m_pParameterMgr->setController(this);
    m_pWidgetFactory = new WidgetFactory(this);
    m_pWidgetFactory->setController(this);
    m_pSelectionMgr = new SelectionMgr(this);
    m_pSelectionMgr->setController(this);
}

//-------------------------------------------------------------------------------------------------

Controller::~Controller()
{

}

//-------------------------------------------------------------------------------------------------

const CXMLNode &Controller::menu1Node() const
{
    return m_pParameterMgr->menu1Node();
}

//-------------------------------------------------------------------------------------------------

const CXMLNode &Controller::menu2Node() const
{
    return m_pParameterMgr->menu2Node();
}

//-------------------------------------------------------------------------------------------------

const CXMLNode &Controller::menu3Node() const
{
    return m_pParameterMgr->menu3Node();
}

//-------------------------------------------------------------------------------------------------

const CXMLNode &Controller::settingsNode() const
{
    return m_pParameterMgr->settingsNode();
}

//-------------------------------------------------------------------------------------------------

ParameterMgr *Controller::parameterMgr() const
{
    return m_pParameterMgr;
}

//-------------------------------------------------------------------------------------------------

WidgetFactory *Controller::widgetFactory() const
{
    return m_pWidgetFactory;
}

//-------------------------------------------------------------------------------------------------

SelectionMgr *Controller::selectionMgr() const
{
    return m_pSelectionMgr;
}

//-------------------------------------------------------------------------------------------------

bool Controller::startup()
{
    if (!m_pParameterMgr->loadMenu1Parameters())
        return false;
    if (!m_pParameterMgr->loadMenu2Parameters())
        return false;
    if (!m_pParameterMgr->loadMenu3Parameters())
        return false;
    if (!m_pParameterMgr->loadSettingsParameters())
        return false;
    return true;
}

//-------------------------------------------------------------------------------------------------

void Controller::shutdown()
{

}

//-------------------------------------------------------------------------------------------------

void Controller::onParameterValueChanged(const QString &sParameterVariable, const QString &sParameterValue)
{
    m_pParameterMgr->setParameterValue(sParameterVariable, sParameterValue);
}

//-------------------------------------------------------------------------------------------------

void Controller::onUpdateWidgetValue(const QString &sParameterVariable, const QString &sVariableValue)
{
    BaseWidget *pWidget = m_pWidgetFactory->getWidgetByVariableName(sParameterVariable);
    if (pWidget != nullptr)
    {
        pWidget->applyValue(sVariableValue);
    }
    else
    {
        qDebug() << "COULD NOT FIND ANY WIDGET WITH ASSOCIATED VARIABLE: " << sParameterVariable;
    }
}

//-------------------------------------------------------------------------------------------------

void Controller::exportParametersToSCAD(const QString &sOutputFileName)
{
   m_pParameterMgr->exportParametersToSCAD(sOutputFileName);
}

//-------------------------------------------------------------------------------------------------

void Controller::exportParametersToTXT(const QString &sOutputFileName)
{
   m_pParameterMgr->exportParametersToTXT(sOutputFileName);
}

//-------------------------------------------------------------------------------------------------

void Controller::importParametersFromTXT(const QString &sInputFileName)
{
    m_pParameterMgr->importParametersFromTXT(sInputFileName);
}
