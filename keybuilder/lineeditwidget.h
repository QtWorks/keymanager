#ifndef LINEEDITWIDGET_H
#define LINEEDITWIDGET_H

// Qt
class QValidator;

// Application
#include "basewidget.h"
class Parameter;
class ParameterMgr;

namespace Ui {
class LineEditWidget;
}

class LineEditWidget : public BaseWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit LineEditWidget(const QString &sLabel, const QString &sDefaultValue, const QString &sAuto, QWidget *parent=nullptr);

    //! Destructor
    virtual ~LineEditWidget();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Set validator
    void setValidator(QValidator *pValidator);

    //! Set label
    void setLabel(const QString &sLabel);

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Apply default value
    virtual void applyDefaultValue();

    //! Set watched parameters
    void setWatchedParameters(const QHash<QString, Parameter *> &hParameters);

private:
    //! UI
    Ui::LineEditWidget *ui;

    //! Auto
    QString m_sAuto;

    //! Watched parameters
    QHash<QString, Parameter *> m_hWatchedParameters;

public slots:
    //! Text changed
    void onTextChanged();

    //! Evaluate auto script
    void onEvaluateAutoScript();
};

#endif // LINEEDITWIDGET_H
