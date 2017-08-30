#ifndef KEYWIDGET_H
#define KEYWIDGET_H

#include <QWidget>

namespace Ui {
class KeyWidget;
}

class KeyWidget : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit KeyWidget(QWidget *parent=nullptr);

    //! Destructor
    ~KeyWidget();

    //! Set title
    void setTitle(const QString &sTitle);

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return value
    QString getValue() const;

private:
    Ui::KeyWidget *ui;
};

#endif // KEYWIDGET_H
