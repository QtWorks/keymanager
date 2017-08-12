// Qt
#include <QDebug>

// Application
#include "parameter.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

Parameter::Parameter(const QString &sName, const QString &sType, const QString &sVariable, const QString &sDefaultValue,
    const QString &sAutoScript, const QString &sEnabledCondition) : m_sName(sName), m_sType(sType), m_sVariable(sVariable),
    m_sValue(sDefaultValue), m_sDefaultValue(sDefaultValue), m_sAutoScript(sAutoScript), m_sEnabledCondition(sEnabledCondition)
{

}

//-------------------------------------------------------------------------------------------------

//! Destructor
Parameter::~Parameter()
{
    qDebug() << "INFORMATION: DESTROY PARAMETER";
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
    qDebug() << "INFORMATION: SETTING VALUE " << sValue << " FOR PARAMETER (" << m_sName << "/" << m_sVariable << ")";
    if (sValue != m_sValue)
    {
        m_sValue = sValue;
        emit parameterValueChanged(m_sVariable, sValue);
    }
}

//-------------------------------------------------------------------------------------------------

const QString &Parameter::defaultValue() const
{
    return m_sDefaultValue;
}

//-------------------------------------------------------------------------------------------------

void Parameter::setDefaultValue(const QString &sDefaultValue)
{
    m_sDefaultValue = sDefaultValue;
}

//-------------------------------------------------------------------------------------------------

const QString &Parameter::autoScript() const
{
    return m_sAutoScript;
}

//-------------------------------------------------------------------------------------------------

void Parameter::setAutoScript(const QString &sAutoScript)
{
    m_sAutoScript = sAutoScript;
}

//-------------------------------------------------------------------------------------------------

const QString &Parameter::enabledCondtion() const
{
    return m_sEnabledCondition;
}

//-------------------------------------------------------------------------------------------------

void Parameter::setEnabledCondition(const QString &sEnabledCondition)
{
    m_sEnabledCondition = sEnabledCondition;
}

//-------------------------------------------------------------------------------------------------

void Parameter::resetToDefaultValue()
{
    setValue(m_sDefaultValue);
}
