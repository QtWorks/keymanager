// Application
#include "doublestatebutton.h"

//-------------------------------------------------------------------------------------------------

DoubleStateButton::DoubleStateButton(QWidget *parent) : QPushButton(parent),
    m_sStateLabel1(""), m_sStateLabel2(""), m_bCurrentState(true)
{
    setState(m_bCurrentState);
    connect(this, &DoubleStateButton::clicked, this, &DoubleStateButton::onClicked);
}

//-------------------------------------------------------------------------------------------------

DoubleStateButton::~DoubleStateButton()
{

}

//-------------------------------------------------------------------------------------------------

bool DoubleStateButton::state() const
{
    return m_bCurrentState;
}

//-------------------------------------------------------------------------------------------------

void DoubleStateButton::setState(bool bOn)
{
    if (bOn != m_bCurrentState)
    {
        m_bCurrentState = bOn;
        setText(m_bCurrentState ? m_sStateLabel1 : m_sStateLabel2);
        emit stateChanged();
    }
}

//-------------------------------------------------------------------------------------------------

void DoubleStateButton::setStateLabels(const QString &sStateLabel1, const QString &sStateLabel2)
{
    m_sStateLabel1 = sStateLabel1;
    m_sStateLabel2 = sStateLabel2;
}

//-------------------------------------------------------------------------------------------------

void DoubleStateButton::onClicked()
{
    emit buttonClicked(m_bCurrentState);
    setState(!m_bCurrentState);
}


