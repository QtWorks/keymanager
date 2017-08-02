#ifndef LINEEDITWIDGET_H
#define LINEEDITWIDGET_H

// Qt
#include <QWidget>

// Application
class QValidator;

namespace Ui {
class LineEditWidget;
}

class LineEditWidget : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit LineEditWidget(const QString &sLabel, QWidget *parent=nullptr);

    //! Destructor
    ~LineEditWidget();

    //! Return value
    QString value() const;

    //! Set validator
    void setValidator(QValidator *pValidator);

private:
    //! UI
    Ui::LineEditWidget *ui;

signals:
    //! Value changed
    void valueChanged();
};

#endif // LINEEDITWIDGET_H
