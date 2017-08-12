// Application
#include "basewidget.h"
#include "controller.h"
#include "parametermgr.h"

//-------------------------------------------------------------------------------------------------

BaseWidget::BaseWidget(Controller *pController, QWidget *parent) : QWidget(parent),
    m_pController(pController), m_sDefaultValue(""), m_sParameterVariable(""), m_sAutoScript("")
{
    connect(this, &BaseWidget::parameterValueChanged, m_pController, &Controller::onParameterValueChanged, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

BaseWidget::~BaseWidget()
{

}

//-------------------------------------------------------------------------------------------------

Controller *BaseWidget::controller() const
{
    return m_pController;
}

//-------------------------------------------------------------------------------------------------

const QString &BaseWidget::defaultValue() const
{
    return m_sDefaultValue;
}

//-------------------------------------------------------------------------------------------------

void BaseWidget::setDefaultValue(const QString &sDefaultValue)
{
    m_sDefaultValue = sDefaultValue;
}

//-------------------------------------------------------------------------------------------------

const QString &BaseWidget::parameterVariable() const
{
    return m_sParameterVariable;
}

//-------------------------------------------------------------------------------------------------

void BaseWidget::setParameterVariable(const QString &sParameterVariable)
{
    m_sParameterVariable = sParameterVariable;
}

//-------------------------------------------------------------------------------------------------

const QString &BaseWidget::autoScript() const
{
    return m_sAutoScript;
}

//-------------------------------------------------------------------------------------------------

void BaseWidget::setAutoScript(const QString &sAutoScript)
{
    m_sAutoScript = sAutoScript;
}

//-------------------------------------------------------------------------------------------------

void BaseWidget::setWatchedParameters(const QHash<QString, Parameter *> &hParameters)
{
    for (QHash<QString, Parameter *>::const_iterator it=hParameters.begin(); it!=hParameters.end(); ++it)
    {
        connect(it.value(), &Parameter::parameterValueChanged, this, &BaseWidget::onEvaluateAutoScript, Qt::UniqueConnection);
        connect(it.value(), &Parameter::parameterValueChanged, this, &BaseWidget::onEvaluateEnabledCondition, Qt::UniqueConnection);
    }
}

//-------------------------------------------------------------------------------------------------

void BaseWidget::applyDefaultValue()
{

}

//-------------------------------------------------------------------------------------------------

void BaseWidget::applyValue(const QString &sValue)
{

}

//-------------------------------------------------------------------------------------------------

void BaseWidget::onEvaluateAutoScript()
{

}

//-------------------------------------------------------------------------------------------------

void BaseWidget::onEvaluateEnabledCondition()
{
    bool bSuccess = true;
    bool bValue = m_pController->parameterMgr()->evaluateEnabledCondition(m_sEnabledCondition, bSuccess);
    if (bSuccess)
        setEnabled(bValue);
}
