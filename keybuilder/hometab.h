#ifndef HOMETAB_H
#define HOMETAB_H

#include <QWidget>

namespace Ui {
class HomeTab;
}

class HomeTab : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit HomeTab(QWidget *parent = 0);

    //! Destructor
    ~HomeTab();

protected:
    //! Paint event
    virtual void paintEvent(QPaintEvent *event);

private:
    //! Load description
    void loadDescription();

private:
    //! UI
    Ui::HomeTab *ui;

    //! App title
    QString m_sAppTitle;

    //! App developer
    QString m_sAppDeveloper;

    //! Developer email
    QString m_sDeveloperEmail;

    //! Developer phone
    QString m_sDeveloperPhone;
};

#endif // HOMETAB_H
