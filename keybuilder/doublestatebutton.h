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
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Set states
    void setStateLabels(const QString &sStateLabel1, const QString &sStateLabel2);

private:
    //! State1
    QString m_sStateLabel1;

    //! State 2
    QString m_sStateLabel2;

public slots:
    //! Clicked
    void onClicked();

signals:
    //! Do action
    void doAction(const QString &sAction);
};

#endif // DOUBLESTATEBUTTON_H
