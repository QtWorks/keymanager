// Qt
#include <QDebug>

// Application
#include "parameter.h"

//-------------------------------------------------------------------------------------------------

Parameter::Parameter() : m_sName(""), m_sType(""), m_sVariable("")
{

}

//-------------------------------------------------------------------------------------------------

Parameter::Parameter(const QString &sName, const QString &sType, const QString &sVariable) : m_sName(sName),
    m_sType(sType), m_sVariable(sVariable)
{

}

//-------------------------------------------------------------------------------------------------

//! Destructor
Parameter::~Parameter()
{
    qDebug() << "*** DESTROY PARAMETER BLOCK ***";
}

//-------------------------------------------------------------------------------------------------

const QString &Parameter::name() const
{
    return m_sName;
}

//-------------------------------------------------------------------------------------------------

void Parameter::setName(const QString &sName)
{
    m_sName = sName;
}

//-------------------------------------------------------------------------------------------------

const QString &Parameter::type() const
{
    return m_sType;
}

//-------------------------------------------------------------------------------------------------

void Parameter::setType(const QString &sType)
{
    m_sType = sType;
}

//-------------------------------------------------------------------------------------------------

const QString &Parameter::variable() const
{
    return m_sVariable;
}

//-------------------------------------------------------------------------------------------------

void Parameter::setVariable(const QString &sVariable)
{
    m_sVariable = sVariable;
}

//-------------------------------------------------------------------------------------------------

const QString &Parameter::value() const
{
    return m_sValue;
}

//-------------------------------------------------------------------------------------------------

void Parameter::setValue(const QString &sValue)
{
    qDebug() << "*** SETTING VALUE " << sValue << " FOR PARAMETER " << m_sName << m_sVariable << " ***";
    m_sValue = sValue;
}
