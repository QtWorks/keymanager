#ifndef PARAMETERBLOCK_H
#define PARAMETERBLOCK_H

// Qt
#include <QWidget>

// Application
#include "cxmlnode.h"
class LayoutMgr;
class ParameterMgr;
class Parameter;

namespace Ui {
class ParameterBlock;
}

class ParameterBlock : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit ParameterBlock(const CXMLNode &xParameterBlock, LayoutMgr *pLayoutMgr, ParameterMgr *pParameterMgr, QWidget *parent=nullptr);

    //! Destructor
    ~ParameterBlock();

    //! Add widget
    void addWidget(QWidget *pWidget, const QString &sParameterVariable);

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return name
    const QString &name() const;

    //! Set name
    void setName(const QString &sName);

    //! Return variable
    const QString &variable() const;

    //! Set variable
    void setVariable(const QString &sVariableName);

    //! Return value
    const QString &value() const;

    //! Set value
    void setValue(const QString &sValue);

    //! Is empty?
    bool isEmpty() const;

    //! Set enabled
    void setEnabled(bool bEnabled);

    //! Is enabled?
    bool isEnabled() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Set watched parameters
    void setWatchedParameters(const QHash<QString, Parameter *> &hParameters);

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! A line edit text changed
    void onLineEditTextChanged();

    //! File picker text changed
    void onFilePickerTextChanged();

    //! A radio button was clicked
    void onRadioButtonClicked(const QString &sSelection);

    //! A line edit triplet value changed
    void onLineEditTripletValueChanged();

    //! Evaluate enabled condition
    void onEvaluateEnabledCondition();

private:
    //! Find associated parameter variable
    QString findAssociatedParameterVariable(QWidget *pWidget) const;

    //! Populate parameter block
    void populateParameterBlock(const CXMLNode &xParameterBlock);

    //! Add widget
    void addWidget(QWidget *pWidget);

private:
    //! UI
    Ui::ParameterBlock *ui;

    //! Name
    QString m_sName;

    //! Widget hash
    QHash<QString, QWidget *> m_hWidgetHash;

    //! Is empty?
    bool m_bIsEmpty;

    //! Layout mgr
    LayoutMgr *m_pLayoutMgr;

    //! Parameter mgr
    ParameterMgr *m_pParameterMgr;

    //! Enabled?
    bool m_bEnabled;

    //! Watched parameters
    QHash<QString, Parameter *> m_vWatchedParameters;

    //! Enabled condition
    QString m_sEnabledCondition;

    //! Watched parameters
    QHash<QString, Parameter *> m_hWatchedParameters;

    //! Variable name
    QString m_sVariableName;

    //! Variable value
    QString m_sValue;

signals:
    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);
};

#endif // PARAMETERBLOCK_H
