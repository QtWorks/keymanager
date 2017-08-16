// Qt
#include <QDebug>

// Application
#include "controller.h"
#include "cxmlnode.h"
#include "parametermgr.h"
#include "widgetfactory.h"
#include "constants.h"
#include "basewidget.h"
#include "genericparametertable.h"
#include "collapsibleblock.h"
#include "helper.h"

//-------------------------------------------------------------------------------------------------

Controller::Controller(QObject *parent) : QObject(parent)
{
    m_pParameterMgr = new ParameterMgr(this);
    connect(m_pParameterMgr, &ParameterMgr::updateWidgetValue, this, &Controller::onUpdateWidgetValue, Qt::UniqueConnection);
    m_pParameterMgr->setController(this);
    m_pWidgetFactory = new WidgetFactory(this);
    m_pWidgetFactory->setController(this);
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
        GenericParameterTable *pTable = dynamic_cast<GenericParameterTable *>(pWidget);
        if (pTable != nullptr)
        {
            pTable->setValue(sParameterVariable, sVariableValue);
        }
        else pWidget->applyValue(sVariableValue);
    }
    else
    {
        Parameter *pParameter = m_pParameterMgr->getParameterByVariableName(sParameterVariable);
        if (pParameter != nullptr)
            pParameter->setValue(sVariableValue);
        else
        {
            QString sMsg = QString("COULD NOT FIND ANY WIDGET OR PARAMETER ASSOCIATED WITH VARIABLE: %1").arg(sParameterVariable);
            logMessage(sMsg);
        }
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
