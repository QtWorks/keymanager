// Application
#include "basewidget.h"
#include "controller.h"

//-------------------------------------------------------------------------------------------------

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent),
    m_pController(nullptr), m_sDefaultValue(""), m_sParameterVariable("")
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

void BaseWidget::applyDefaultValue()
{

}

