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
    explicit DoubleTripletWidget(QWidget *parent=nullptr);

    //! Constructor
    explicit DoubleTripletWidget(const QString &sLabel, const QString &sDefaultValue, QWidget *parent=nullptr);

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

private:
    //! UI
    Ui::DoubleTripletWidget *ui;

signals:
    //! Value changed
    void valueChanged();
};

#endif // DOUBLETRIPLETWIDGET_H
