#ifndef BASEWIDGET_H
#define BASEWIDGET_H

// Qt
#include <QWidget>

// Application
class Controller;

class BaseWidget : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit BaseWidget(QWidget *parent=nullptr);

    //! Destructor
    virtual ~BaseWidget();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return controller
    Controller *controller() const;

    //! Set controller
    void setController(Controller *pController);

    //! Return default value
    const QString &defaultValue() const;

    //! Set default value
    void setDefaultValue(const QString &sDefaultValue);

    //! Return parameter variable
    const QString &parameterVariable() const;

    //! Set parameter variable
    void setParameterVariable(const QString &sParameterVariable);

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Apply default value
    virtual void applyDefaultValue();

protected:
    //! Controller
    Controller *m_pController;

    //! Default value
    QString m_sDefaultValue;

    //! Parameter variable
    QString m_sParameterVariable;

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);
};

#endif // BASEWIDGET_H
