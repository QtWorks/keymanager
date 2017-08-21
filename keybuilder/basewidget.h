#ifndef BASEWIDGET_H
#define BASEWIDGET_H

// Qt
#include <QWidget>

// Application
class Controller;
class Parameter;

class BaseWidget : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit BaseWidget(Controller *pController, QWidget *parent=nullptr);

    //! Destructor
    virtual ~BaseWidget();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return controller
    Controller *controller() const;

    //! Return default value
    const QString &defaultValue() const;

    //! Set default value
    void setDefaultValue(const QString &sDefaultValue);

    //! Return parameter variable
    const QString &parameterVariable() const;

    //! Set parameter variable
    void setParameterVariable(const QString &sParameterVariable);

    //! Return auto script
    const QString &autoScript() const;

    //! Set auto script
    void setAutoScript(const QString &sAutoScript);

    //! Return enabled condition
    const QString &enabledCondition() const;

    //! Set enabled condition
    void setEnabledCondition(const QString &sEnabledCondition);

    //! Set watched parameters
    void setWatchedParameters(const QHash<QString, Parameter *> &hParameters);

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Apply default value
    virtual void applyDefaultValue();

    //! Clear all (used for tables)
    virtual void clearAll();

    //! Apply value
    virtual void applyValue(const QString &sValue);

protected:
    //! Controller
    Controller *m_pController;

private:
    //! Default value
    QString m_sDefaultValue;

    //! Parameter variable
    QString m_sParameterVariable;

    //! Auto script
    QString m_sAutoScript;

    //! Enabled condition
    QString m_sEnabledCondition;

public slots:
    //! Evaluate auto script
    virtual void onEvaluateAutoScript();

    //! Evaluate enabled condition
    virtual void onEvaluateEnabledCondition();

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);
};

#endif // BASEWIDGET_H
