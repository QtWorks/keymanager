// Application
#include "doublevalidator.h"

//-------------------------------------------------------------------------------------------------

DoubleValidator::DoubleValidator(double dMin, double dMax, int iDecimals, QObject *parent) : QValidator(parent),
    m_dMin(dMin), m_dMax(dMax), m_iDecimals(iDecimals)
{

}

//-------------------------------------------------------------------------------------------------

DoubleValidator::~DoubleValidator()
{

}

//-------------------------------------------------------------------------------------------------

QValidator::State DoubleValidator::validate(QString &sInput, int &iPos) const
{
    Q_UNUSED(iPos);
    if (sInput.isEmpty())
        return QValidator::Acceptable;
    if (sInput.contains(","))
        return QValidator::Invalid;
    bool bOK = true;
    double dConverted = sInput.toDouble(&bOK);
    if (!bOK)
        return QValidator::Invalid;
    return (dConverted >= m_dMin) && (dConverted <= m_dMax) ? QValidator::Acceptable : QValidator::Invalid;
}
