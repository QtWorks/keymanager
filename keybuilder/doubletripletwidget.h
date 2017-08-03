#ifndef DOUBLETRIPLETWIDGET_H
#define DOUBLETRIPLETWIDGET_H

#include <QWidget>

namespace Ui {
class DoubleTripletWidget;
}

class DoubleTripletWidget : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit DoubleTripletWidget(QWidget *parent=nullptr);

    //! Constructor
    explicit DoubleTripletWidget(const QString &sLabel, QWidget *parent=nullptr);

    //! Destructor
    ~DoubleTripletWidget();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Set label
    void setLabel(const QString &sLabel);

    //! Return value
    QString value() const;

private:
    //! UI
    Ui::DoubleTripletWidget *ui;

signals:
    //! Value changed
    void valueChanged();
};

#endif // DOUBLETRIPLETWIDGET_H
