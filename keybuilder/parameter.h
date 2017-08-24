#ifndef PARAMETER_H
#define PARAMETER_H

// Qt
#include <QString>
#include <QVariant>

class Parameter : public QObject
{
    Q_OBJECT

public:
    friend class ParameterMgr;

    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    Parameter(const QString &sName, const QString &sType, const QString &sVariable, const QString &sDefaultValue, const QString &sAutoScript, const QString &sEnabledCondition, bool bSetValue=true);

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

    //! Get default value
    const QString &defaultValue() const;

    //! Set default value
    void setDefaultValue(const QString &sDefaultValue);

    //! Get unset value (used for tables only)
    const QString &unsetValue() const;

    //! Set unser value
    void setUnsetValue(const QString &sUnsetValue);

    //! Get auto script
    const QString &autoScript() const;

    //! Set auto script
    void setAutoScript(const QString &sAutoScript);

    //! Get enabled condition
    const QString &enabledCondtion() const;

    //! Set enabled condition
    void setEnabledCondition(const QString &sEnabledCondition);

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Reset to default value
    void resetToDefaultValue();

private:
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

    //! Default value
    QString m_sDefaultValue;

    //! Unset value
    QString m_sUnsetValue;

    //! Auto script
    QString m_sAutoScript;

    //! Enabled condition
    QString m_sEnabledCondition;

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);
};

#endif // PARAMETER_H
