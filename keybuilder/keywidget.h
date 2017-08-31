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

    //! Set read only
    void setReadOnly(bool bReadOnly);

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return value
    QString getValue() const;

    //! Set value
    void setValue(const QString &sValue);

private:
    //! Apply font
    void applyFont();

private:
    //! UI
    Ui::KeyWidget *ui;
};

#endif // KEYWIDGET_H
