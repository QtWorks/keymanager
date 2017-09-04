#ifndef DOUBLESTATEBUTTON_H
#define DOUBLESTATEBUTTON_H

// Qt
#include <QPushButton>

class DoubleStateButton : public QPushButton
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    DoubleStateButton(QWidget *parent=nullptr);

    //! Destructor
    ~DoubleStateButton();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return state
    bool state() const;

    //! Set state
    void setState(bool bOn);

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Set states
    void setStateLabels(const QString &sStateLabel1, const QString &sStateLabel2);

private:
    //! State1
    QString m_sStateLabel1;

    //! State 2
    QString m_sStateLabel2;

    //! Current state
    bool m_bCurrentState;

public slots:
    //! Clicked
    void onClicked();

signals:
    //! Button clicked
    void buttonClicked(bool bButtonClicked);

    //! State changed
    void stateChanged();
};

#endif // DOUBLESTATEBUTTON_H
