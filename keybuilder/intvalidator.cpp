// Application
#include "intvalidator.h"

//-------------------------------------------------------------------------------------------------

IntValidator::IntValidator(int iMin, int iMax, QObject *parent) : QIntValidator(iMin, iMax, parent)
{

}

//-------------------------------------------------------------------------------------------------

IntValidator::~IntValidator()
{

}

//-------------------------------------------------------------------------------------------------

QValidator::State IntValidator::validate(QString &sInput, int &iPos) const
{
    QIntValidator::State eResult(QIntValidator::validate(sInput, iPos));
    if (sInput.isEmpty())
        return QValidator::Acceptable;
    if (eResult == QValidator::Intermediate)
        eResult = QValidator::Invalid;
    return eResult;
}
