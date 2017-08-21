#ifndef GENERICPARAMETERTABLE_H
#define GENERICPARAMETERTABLE_H

// Qt
#include <QWidget>
#include <QAbstractItemModel>
#include <QVector>
#include <QItemDelegate>

// Application
#include "basewidget.h"
class Controller;

typedef struct P {
    int column;
    int row;
} Position;

namespace Ui {
class GenericParameterTable;
}

class GenericParameterTableModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    GenericParameterTableModel(Controller *pController, const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QString &sDefaultValue, const QString &sTargetRow,
        int nRows, const QString &sTargetVariable, const QString &sVariableMethod, const QString &sActionSetNumberOfPins, QObject *parent=nullptr);

    //! Destructor
    virtual ~GenericParameterTableModel();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return column labels
    const QStringList &columnLabels() const;

    //! Return column variables
    const QStringList &columnVariables() const;

    //! Return parameter variable hash table
    QHash<QString, Position> parameterVariableHashTable() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Return index
    QModelIndex index(int iRow, int iCol, const QModelIndex & parent=QModelIndex()) const;

    //! Return index
    QModelIndex parent(const QModelIndex &) const;

    //! Return row count
    int rowCount(const QModelIndex &parent=QModelIndex()) const;

    //! Return column count
    int columnCount(const QModelIndex &parent=QModelIndex()) const;

    //! Return data
    QVariant data(const QModelIndex &index, int iRole=Qt::DisplayRole) const;

    //! Set data
    virtual bool setData(const QModelIndex &index, const QVariant &, int iRole=Qt::EditRole);

    //! Return header data
    virtual QVariant headerData(int section, Qt::Orientation eOrientation, int iRole=Qt::DisplayRole) const;

    //! Return flags
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Clear all
    void clearAll();

    //! Apply value
    void applyValue(const QString &sValue, int iTargetColumn=-1);

    //! Set value
    void setValue(const QString &sParameterVariable, const QString &sVariableValue);

    //! Evaluate full script
    void evaluateAutoScript(const QString &sFullScript);

    //! Evaluate auto script
    void evaluateSingleScript(const QString &sAutoScript);

private:
    //! Get formatted variable name
    static QString getFormattedVariableName(const QString &sVariableMethod, const QString &sTargetVariable, const QStringList &lColumnVariables, const QString &sTargetRow, int iColumn, int iRow);

    //! Process action set number of pins
    void processActionSetNumberOfPins(const QString &sActionSetNumberOfPins);

private:
    //! Controller
    Controller *m_pController;

    //! Default value
    QString m_sDefaultValue;

    //! Default values
    QStringList m_lDefaultValues;

    //! Column labels
    QStringList m_lColumnLabels;

    //! Column variables
    QStringList m_lColumnVariables;

    //! Target row
    QString m_sTargetRow;

    //! Number of rows
    int m_nRows;

    //! Number of enabled rows
    int m_nEnabledRows;

    //! Target variabe
    QString m_sTargetVariable;

    //! Variable method
    QString m_sVariableMethod;

    //! Action set number of pins
    QString m_sActionSetNumberOfPins;

    //! Data
    QVector<QString> m_vData;

    //! QHash variable/(row, column)
    QHash<QString, Position> m_hHashTable;

public slots:
    //! Set number of rows
    void onSetNumberOfRows(const QString &sParameterName, const QString &sParameterValue);

    //! Update all
    void onUpdateAll(int iTargetColumn);

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);

    //! Update all
    void updateAll(int iTargetColumn);
};

class ItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    ItemDelegate(QObject *parent=nullptr);

    //! Destructor
    ~ItemDelegate();

    //! Create editor
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    //! Set editor data
    void setEditorData(QWidget *pEditor, const QModelIndex &index) const;

    //! Set model data
    void setModelData(QWidget *pEditor, QAbstractItemModel *pModel, const QModelIndex &index) const;

    //! Update editor geometry
    void updateEditorGeometry(QWidget *pEditor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class GenericParameterTable : public BaseWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit GenericParameterTable(Controller *pController, const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QString &sDefaultValue, const QString &sTargetRow,
        int nRows, const QString &sTargetVariable,  const QString &sVariableMethod,
            const QString &sActionSetNumberOfPins, const QString &setAutoScript, QWidget *parent=nullptr);

    //! Destructor
    ~GenericParameterTable();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return parameter variable hash table
    QHash<QString, Position> parameterVariableHashTable() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Apply default values
    virtual void applyDefaultValue();

    //! Clear all
    virtual void clearAll();

    //! Apply value
    virtual void applyValue(const QString &sValue);

    //! Set value
    void setValue(const QString &sParameterVariable, const QString &sVariableValue);

private:
    //! UI
    Ui::GenericParameterTable *ui;

    //! Model
    GenericParameterTableModel *m_pModel;

public slots:
    //! Evaluate auto script
    virtual void onEvaluateAutoScript();

    //! Clear column
    void onClearColumn(int iTargetColumn);
};

#endif // GENERICPARAMETERTABLE_H
