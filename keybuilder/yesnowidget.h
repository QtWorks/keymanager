#ifndef YESNOWIDGET_H
#define YESNOWIDGET_H

// Application
#include "basewidget.h"
class QRadioButton;
namespace Ui {
class YesNoWidget;
}

class YesNoWidget : public BaseWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit YesNoWidget(Controller *pController, const QString &sLabel, const QString &sDefaultValue, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent);

    //! Destructor
    virtual ~YesNoWidget();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Apply default value
    virtual void applyDefaultValue();

    //! Apply value
    virtual void applyValue(const QString &sValue);

private:
    //! UI
    Ui::YesNoWidget *ui;

public slots:
    //! Radio button clicked
    void onRadioButtonToggled(bool bClicked);

signals:
    //! Selection changed
    void selectionChanged(const QString &sSelection);
};

#endif // YESNOWIDGET_H
