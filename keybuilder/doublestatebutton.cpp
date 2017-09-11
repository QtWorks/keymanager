// Qt
#include <QDebug>

// Application
#include "doublestatebutton.h"

//-------------------------------------------------------------------------------------------------

DoubleStateButton::DoubleStateButton(QWidget *parent) : QPushButton(parent),
    m_sStateLabel1(""), m_sStateLabel2("")
{
    connect(this, &DoubleStateButton::clicked, this, &DoubleStateButton::onClicked, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

DoubleStateButton::~DoubleStateButton()
{

}

//-------------------------------------------------------------------------------------------------

void DoubleStateButton::setStateLabels(const QString &sStateLabel1, const QString &sStateLabel2)
{
    m_sStateLabel1 = sStateLabel1;
    m_sStateLabel2 = sStateLabel2;
    setText(m_sStateLabel1);
}

//-------------------------------------------------------------------------------------------------

void DoubleStateButton::onClicked()
{
    QString sCurrentText = text();
    if (text() == m_sStateLabel1)
        setText(m_sStateLabel2);
    else
        setText(m_sStateLabel1);
    emit doAction(sCurrentText);
}


