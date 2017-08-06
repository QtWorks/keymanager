// Qt
#include <QDebug>

// Application
#include "controller.h"
#include "cxmlnode.h"
#include "parametermgr.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

Controller::Controller(QObject *parent) : QObject(parent)
{
    m_pParameterMgr = new ParameterMgr(this);
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

ParameterMgr *Controller::parameterMgr() const
{
    return m_pParameterMgr;
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
    return true;
}

//-------------------------------------------------------------------------------------------------

void Controller::shutdown()
{

}

//-------------------------------------------------------------------------------------------------

void Controller::onParameterValueChanged(const QString &sParameterName, const QString &sParameterValue)
{
    m_pParameterMgr->setParameterValue(sParameterName, sParameterValue);
}

//-------------------------------------------------------------------------------------------------

void Controller::onGenerateScript()
{
    m_pParameterMgr->generateScript();
}
