// Application
#include "doublevalidator.h"

//-------------------------------------------------------------------------------------------------

DoubleValidator::DoubleValidator(double iMin, double iMax, int iDecimals, QObject *parent) : QDoubleValidator(iMin, iMax, iDecimals, parent)
{

}

//-------------------------------------------------------------------------------------------------

DoubleValidator::~DoubleValidator()
{

}

//-------------------------------------------------------------------------------------------------

QValidator::State DoubleValidator::validate(QString &sInput, int &iPos) const
{
    QDoubleValidator::State eResult(QDoubleValidator::validate(sInput, iPos));
    if (sInput.isEmpty())
        return QValidator::Acceptable;
    if (eResult == QValidator::Intermediate)
        eResult = QValidator::Invalid;
    return eResult;
}
