#ifndef DOUBLEVALIDATOR_H
#define DOUBLEVALIDATOR_H

// Qt
#include <QDoubleValidator>

class DoubleValidator : public QDoubleValidator
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    DoubleValidator(double iMin, double iMax, int iDecimals, QObject *parent=nullptr);

    //! Destructor
    ~DoubleValidator();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Validate
    virtual QValidator::State validate(QString &sInput, int &iPos) const;
};

#endif // DOUBLEVALIDATOR_H
