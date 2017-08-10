// Application
#include "basewidget.h"
#include "controller.h"
#include "parametermgr.h"

//-------------------------------------------------------------------------------------------------

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent),
    m_pController(nullptr), m_sDefaultValue(""), m_sParameterVariable(""), m_sAutoScript("")
{

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

void BaseWidget::setController(Controller *pController)
{
    m_pController = pController;
    connect(this, &BaseWidget::parameterValueChanged, m_pController, &Controller::onParameterValueChanged);
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
    for (QHash<QString, Parameter *>::const_iterator it=hParameters.begin(); it!=hParameters.end(); ++it) {
        connect(it.value(), &Parameter::parameterValueChanged, this, &BaseWidget::onEvaluateAutoScript);
        connect(it.value(), &Parameter::parameterValueChanged, this, &BaseWidget::onEvaluateEnabledCondition);
    }
}

//-------------------------------------------------------------------------------------------------

void BaseWidget::applyDefaultValue()
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
