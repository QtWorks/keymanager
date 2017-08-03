#ifndef LEVERTABLEWIDGET_H
#define LEVERTABLEWIDGET_H

// Qt
#include <QWidget>
#include <QAbstractItemModel>
#include <QVector>
#include <QItemDelegate>

namespace Ui {
class LeverTableWidget;
}

class LeverTableModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    LeverTableModel(const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QString &sTargetRow,
        int nRows, const QString &sTargetVariable,  QObject *parent=nullptr);

    //! Destructor
    ~LeverTableModel();

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

private:
    //! Identify target variable
    QString identifyTargetVariable(int iColumn, int iRow) const;

private:
    //! Column labels
    QStringList m_lColumnLabels;

    //! Column variables
    QStringList m_lColumnVariables;

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

class LeverTableWidget : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit LeverTableWidget(const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QString &sTargetRow,
        int nRows, const QString &sTargetVariable,  QWidget *parent=nullptr);

    //! Destructor
    ~LeverTableWidget();

private:
    //! Populate button area
    void populateButtonArea();

    //! Reset column variables
    void resetColumnVariables(int iColumnIndex);

private:
    //! UI
    Ui::LeverTableWidget *ui;

    //! Model
    LeverTableModel *m_pModel;

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

#endif // LEVERTABLEWIDGET_H
