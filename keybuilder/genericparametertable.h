#ifndef GENERICPARAMETERTABLE_H
#define GENERICPARAMETERTABLE_H

// Qt
#include <QWidget>
#include <QAbstractItemModel>
#include <QVector>
#include <QItemDelegate>

// Application
#include "basewidget.h"

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
    GenericParameterTableModel(const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QStringList &lDefaultValues, const QString &sTargetRow,
        int nRows, const QString &sTargetVariable, const QString &sVariableMethod, QObject *parent=nullptr);

    //! Destructor
    virtual ~GenericParameterTableModel();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return column labels
    const QStringList &columnLabels() const;

    //! Return column variables
    const QStringList &columnVariables() const;

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
    virtual QVariant  headerData(int section, Qt::Orientation eOrientation, int iRole=Qt::DisplayRole) const;

    //! Return flags
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Set number of active lever
    void setNumActiveLever(int iNumActiveLever);

    //! Reset column variables
    void resetColumnVariables(int iColumnIndex);

    //! Set default values
    void applyDefaultValue();

private:
    //! Column labels
    QStringList m_lColumnLabels;

    //! Column variables
    QStringList m_lColumnVariables;

    //! Default values
    QStringList m_lDefaultValues;

    //! Target row
    QString m_sTargetRow;

    //! Number of rows
    int m_nRows;

    //! Number of active lever
    int m_nActiveLever;

    //! Number of enabled rows
    int m_nEnabledRows;

    //! Target variabe
    QString m_sTargetVariable;

    //! Variable method
    QString m_sVariableMethod;

    //! Data
    QVector<double> m_vData;

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);
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
    explicit GenericParameterTable(const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QStringList &lDefaultValues, const QString &sTargetRow,
        int nRows, const QString &sTargetVariable,  const QString &sVariableMethod, QWidget *parent=nullptr);

    //! Destructor
    ~GenericParameterTable();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Apply default values
    virtual void applyDefaultValue();

private:
    //! Populate button area
    void populateButtonArea();

    //! Reset column variables
    void resetColumnVariables(int iColumnIndex);

private:
    //! UI
    Ui::GenericParameterTable *ui;

    //! Model
    GenericParameterTableModel *m_pModel;

public slots:
    //! Number of active lever changed
    void onNumActiveLeverChanged();

    //! Action button clicked
    void onActionButtonClicked();

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);
};

#endif // GENERICPARAMETERTABLE_H
