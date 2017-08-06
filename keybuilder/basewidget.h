#ifndef BASEWIDGET_H
#define BASEWIDGET_H

// Qt
#include <QWidget>

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

    //! Apply default value
    virtual void applyDefaultValue();

protected:
    //! Default value
    QString m_sDefaultValue;
};

#endif // BASEWIDGET_H
