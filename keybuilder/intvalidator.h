#ifndef INTVALIDATOR_H
#define INTVALIDATOR_H

// Qt
#include <QIntValidator>

class IntValidator : public QIntValidator
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    IntValidator(int iMin, int iMax, QObject *parent=nullptr);

    //! Destructor
    ~IntValidator();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Validate
    virtual QValidator::State validate(QString &sInput, int &iPos) const;
};

#endif // INTVALIDATOR_H
