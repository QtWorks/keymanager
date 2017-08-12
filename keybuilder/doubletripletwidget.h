#ifndef DOUBLETRIPLETWIDGET_H
#define DOUBLETRIPLETWIDGET_H

// Application
#include "basewidget.h"

namespace Ui {
class DoubleTripletWidget;
}

class DoubleTripletWidget : public BaseWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit DoubleTripletWidget(Controller *pController, const QString &sLabel, const QString &sDefaultValue, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent=nullptr);

    //! Destructor
    virtual ~DoubleTripletWidget();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Set label
    void setLabel(const QString &sLabel);

    //! Return value
    QString value() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Apply default value
    virtual void applyDefaultValue();

    //! Apply value
    virtual void applyValue(const QString &sValue);

private:
    //! UI
    Ui::DoubleTripletWidget *ui;

public slots:
    //! Text changed
    void onTextChanged();
};

#endif // DOUBLETRIPLETWIDGET_H
