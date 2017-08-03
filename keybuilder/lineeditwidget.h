#ifndef LINEEDITWIDGET_H
#define LINEEDITWIDGET_H

// Qt
#include <QWidget>
class QValidator;

// Application
class Parameter;
class ParameterMgr;

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
    explicit LineEditWidget(QWidget *parent=nullptr);

    //! Constructor
    explicit LineEditWidget(const QString &sLabel, const QString &sDefaultValue, const QString &sAuto, QWidget *parent=nullptr);

    //! Destructor
    ~LineEditWidget();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return value
    QString value() const;

    //! Set validator
    void setValidator(QValidator *pValidator);

    //! Set label
    void setLabel(const QString &sLabel);

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Apply default value
    void applyDefaultValue();

    //! Set watched parameters
    void setWatchedParameters(const QHash<QString, Parameter *> &hParameters);

    //! Set parameter mgr
    void setParameterMgr(ParameterMgr *pParameterMgr);

private:
    //! UI
    Ui::LineEditWidget *ui;

    //! Default value
    QString m_sDefaultValue;

    //! Auto
    QString m_sAuto;

    //! Watched parameters
    QHash<QString, Parameter *> m_hWatchedParameters;

    //! Parameter mgr
    ParameterMgr *m_pParameterMgr;

public slots:
    //! Evaluate auto script
    void onEvaluateAutoScript();
    
signals:
    //! Value changed
    void valueChanged();
};

#endif // LINEEDITWIDGET_H
