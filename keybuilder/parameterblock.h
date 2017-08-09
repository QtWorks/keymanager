#ifndef PARAMETERBLOCK_H
#define PARAMETERBLOCK_H

// Qt
#include <QWidget>

// Application
#include "cxmlnode.h"
class Controller;
class Parameter;
class BaseWidget;
class CollapsibleBlock;

namespace Ui {
class ParameterBlock;
}

class ParameterBlock : public QWidget
{
    Q_OBJECT

public:
    friend class CollapsibleBlock;

    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Destructor
    ~ParameterBlock();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return name
    const QString &name() const;

    //! Set name
    void setName(const QString &sName);

    //! Return selection variable
    const QString &selectionVariable() const;

    //! Set variable
    void setSelectionVariable(const QString &sVariableName);

    //! Return value
    const QString &value() const;

    //! Set value
    void setValue(const QString &sValue);

    //! Is empty?
    bool isEmpty() const;

    //! Set enabled
    void setEnabled(bool bEnabled);

    //! Is enabled
    bool isEnabled() const;

    //! Set exclusive
    void setExclusive(bool bIsExclusive);

    //! Exclusive?
    bool isExclusive() const;

    //! Return owner collapsible block
    CollapsibleBlock *ownerCollapsibleBlock() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Set watched parameters
    void setWatchedParameters(const QHash<QString, Parameter *> &hParameters);

    //! Clear all
    void clearAll();

    //! Add child recursively
    void addChildRecursively(const CXMLNode &xParameterBlock);

    //! Add widget
    void addWidget(BaseWidget *pWidget);

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Evaluate enabled condition
    void onEvaluateEnabledCondition();

private:
    //! Constructor
    explicit ParameterBlock(const CXMLNode &xParameterBlock, CollapsibleBlock *pOwner, Controller *pController, QWidget *parent=nullptr);

    //! Populate parameter block
    void populateParameterBlock(const CXMLNode &xParameterBlock);

    //! Add collapsible block
    void addCollapsibleBlock(CollapsibleBlock *pBlock);

private:
    //! UI
    Ui::ParameterBlock *ui;

    //! Name
    QString m_sName;

    //! Is empty?
    bool m_bIsEmpty;

    //! Enabled condition
    QString m_sEnabledCondition;

    //! Watched parameters
    QHash<QString, Parameter *> m_hWatchedParameters;

    //! Variable name
    QString m_sSelectionVariable;

    //! Variable value
    QString m_sValue;

    //! Exclusive?
    bool m_bIsExclusive;

    //! Enabled
    bool m_bIsEnabled;

    //! Owner collapsible block
    CollapsibleBlock *m_pOwnerCollapsibleBlock;

    //! Controller
    Controller *m_pController;

    //! Widgets
    QVector<BaseWidget *> m_vWidgets;

signals:
    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);
};

#endif // PARAMETERBLOCK_H
