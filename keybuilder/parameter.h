#ifndef PARAMETER_H
#define PARAMETER_H

// Qt
#include <QString>
#include <QVariant>

class Parameter
{
public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    Parameter();

    //! Constructor
    Parameter(const QString &sName, const QString &sType, const QString &sVariable);

    //! Destructor
    ~Parameter();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Get name
    const QString &name() const;

    //! Set name
    void setName(const QString &sName);

    //! Get type
    const QString &type() const;

    //! Set type
    void setType(const QString &sType);

    //! Get variable
    const QString &variable() const;

    //! Set variable
    void setVariable(const QString &sVariable);

    //! Get value
    const QString &value() const;

    //! Set value
    void setValue(const QString &sValue);

private:
    //! Name
    QString m_sName;

    //! Type
    QString m_sType;

    //! Associated variable
    QString m_sVariable;

    //! Value
    QString m_sValue;
};

#endif // PARAMETER_H
