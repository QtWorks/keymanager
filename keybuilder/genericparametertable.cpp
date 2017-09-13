// Qt
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QScriptEngine>
#include <QScriptValue>

// Application
#include "genericparametertable.h"
#include "ui_genericparametertable.h"
#include "controller.h"
#include "parametermgr.h"
#include "constants.h"
#include "intvalidator.h"
#include "helper.h"
#include "customheaderview.h"
#include "doublevalidator.h"

//-------------------------------------------------------------------------------------------------

GenericParameterTableModel::GenericParameterTableModel(Controller *pController, const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QString &sDefaultValue, const QString &sTargetRow,
    int nRows, const QString &sTargetVariable, const QString &sVariableMethod, const QString &sActionSetNumberOfRows, QObject *parent) : QAbstractItemModel(parent),
    m_pController(pController), m_sTargetRow(""), m_nMaxNumberOfRows(0), m_nRows(0), m_sTargetVariable(""), m_sVariableMethod("")
{
    int nColumns = qMin(lColumnLabels.size(), lColumnVariables.size());

    // Check we have the right number of default values:
    QStringList lDefaultValues;
    if (sDefaultValue.isEmpty())
    {
        for (int i=0; i<nColumns; i++)
            lDefaultValues << VALUE_DEFAULT_VALUE;
    }
    else
    if (sDefaultValue.contains(","))
    {
        lDefaultValues = sDefaultValue.split(",");
        if (lDefaultValues.size() != nColumns)
        {
            lDefaultValues.clear();
            for (int i=0; i<nColumns; i++)
                lDefaultValues << VALUE_DEFAULT_VALUE;
        }
    }
    else lDefaultValues << sDefaultValue;

    if (nColumns > 0)
    {
        // Set default values
        m_lDefaultValues = lDefaultValues;

        // Set own properties
        m_lColumnLabels = lColumnLabels.mid(0, nColumns);
        m_lColumnVariables = lColumnVariables.mid(0, nColumns);
        m_sTargetRow = sTargetRow;
        m_nMaxNumberOfRows = nRows;
        m_nRows = nRows;
        m_sTargetVariable = sTargetVariable;
        m_sVariableMethod = sVariableMethod;
        if (!sActionSetNumberOfRows.isEmpty())
            processActionSetNumberOfRows(sActionSetNumberOfRows);
        m_vData.resize((nRows+1)*nColumns);

        // Write first row using default values
        for (int i=0; i<m_lDefaultValues.size(); i++)
            m_vData[i] = m_lDefaultValues[i];

        for (int i=0; i<nRows; i++)
        {
            for (int j=0; j<nColumns; j++)
            {
                // Build formatted variable name
                QString sFormattedVariableName = getFormattedVariableName(m_sVariableMethod, sTargetVariable, m_lColumnVariables, m_sTargetRow, j, i);
                Position p;
                p.column = j;
                p.row = i+1;
                m_hHashTable[sFormattedVariableName] = p;
            }
        }
        connect(this, &GenericParameterTableModel::updateAll, this, &GenericParameterTableModel::onUpdateAll, Qt::UniqueConnection);
    }
    else logError("CANNOT CREATE A TABLE WITH 0 COLUMN");
}

//-------------------------------------------------------------------------------------------------

GenericParameterTableModel::~GenericParameterTableModel()
{

}

//-------------------------------------------------------------------------------------------------

const QStringList &GenericParameterTableModel::columnLabels() const
{
    return m_lColumnLabels;
}

//-------------------------------------------------------------------------------------------------

const QStringList &GenericParameterTableModel::columnVariables() const
{
    return m_lColumnVariables;
}

//-------------------------------------------------------------------------------------------------

QHash<QString, Position> GenericParameterTableModel::parameterVariableHashTable() const
{
    return m_hHashTable;
}

//-------------------------------------------------------------------------------------------------

QModelIndex GenericParameterTableModel::index(int iRow, int iCol, const QModelIndex &parent) const
{
    if (!parent.isValid())
        return createIndex(iRow, iCol);
    return QModelIndex();
}

//-------------------------------------------------------------------------------------------------

QModelIndex GenericParameterTableModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

//-------------------------------------------------------------------------------------------------

int GenericParameterTableModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_nRows+1;
    return 0;
}

//-------------------------------------------------------------------------------------------------

int GenericParameterTableModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_lColumnLabels.size();
    return 0;
}

//-------------------------------------------------------------------------------------------------

QVariant GenericParameterTableModel::data(const QModelIndex &index, int iRole) const
{
    if (index.isValid())
    {
        if (iRole == Qt::DisplayRole)
        {
            int iTargetRow = index.row();
            QString sTargetData("");
            if (iTargetRow > 0)
            {
                int iIndex = index.column()+iTargetRow*m_lColumnVariables.size();
                if ((iIndex >= 0) && (iIndex < m_vData.size()))
                    sTargetData = m_vData[iIndex];
            }
            else
            {
                if ((index.column() >= 0) && (index.column() < m_vData.size()))
                    sTargetData = m_vData[index.column()];
            }
            double d = sTargetData.toDouble();
            return sTargetData.isEmpty() ? sTargetData : QString::number(d, 'f', 4);
        }
        else if (iRole == Qt::BackgroundColorRole)
            return (index.row()%2 == 0) ? QColor(Qt::white) : QColor(Qt::lightGray);
    }
    return QVariant();
}

//-------------------------------------------------------------------------------------------------

bool GenericParameterTableModel::setData(const QModelIndex &index, const QVariant &vData, int iRole)
{
    if (index.isValid() && (iRole == Qt::EditRole))
    {
        QString sValue = vData.toString();
        int iTargetRow = index.row();
        if (iTargetRow > 0)
        {
            int iTargetIndex = index.column()+iTargetRow*m_lColumnVariables.size();
            if ((iTargetIndex >= 0) && (iTargetIndex < m_vData.size()))
            {
                m_vData[index.column()+iTargetRow*m_lColumnVariables.size()] = sValue;
                QString sFormattedVariableName = getFormattedVariableName(m_sVariableMethod, m_sTargetVariable, m_lColumnVariables, m_sTargetRow, index.column(), iTargetRow-1);
                emit parameterValueChanged(sFormattedVariableName, vData.toString());
                emit dataChanged(index, index, QVector<int>() << Qt::DisplayRole);
            }
        }
        else
        {
            if ((index.column() >= 0) && (index.column() < m_vData.size()))
            {
                m_vData[index.column()] = sValue;
                emit dataChanged(index, index, QVector<int>() << Qt::DisplayRole);
                emit updateAll(index.column());
            }
        }
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------

QVariant GenericParameterTableModel::headerData(int section, Qt::Orientation eOrientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (eOrientation == Qt::Horizontal)
        {
            return m_lColumnLabels[section];
        }
        else
        if (eOrientation == Qt::Vertical)
        {
            if (section == 0)
                return QString("ALL");
            else return section;
        }
    }

    return QAbstractItemModel::headerData(section, eOrientation, role);
}

//-------------------------------------------------------------------------------------------------

Qt::ItemFlags GenericParameterTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    return flags|Qt::ItemIsEditable;
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::clearAll()
{
    beginResetModel();
    // Reset first row to default values
    for (int i=0; i<m_lDefaultValues.size(); i++)
        m_vData[i] = m_lDefaultValues[i];
    // Clear other rows
    for (int i=m_lDefaultValues.size(); i<m_vData.size(); i++)
        m_vData[i] = QString("");
    endResetModel();
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::applyValue(const QString &sValue, int iTargetColumn)
{
    QStringList lValues;
    if (sValue.contains(","))
        lValues = sValue.split(",");
    else
        lValues << sValue;

    if (!lValues.isEmpty())
    {
        for (int i=0; i<m_nRows+1; i++)
        {
            if ((iTargetColumn >= 0) && (iTargetColumn < lValues.size()))
            {
                QModelIndex targetIndex = index(i, iTargetColumn, QModelIndex());
                if (targetIndex.isValid())
                    setData(targetIndex, lValues[iTargetColumn], Qt::EditRole);
            }
            else
            for (int j=0; j<lValues.size(); j++)
            {
                QModelIndex targetIndex = index(i, j, QModelIndex());
                if (targetIndex.isValid())
                    setData(targetIndex, lValues[j], Qt::EditRole);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::clearColumn(int iTargetColumn)
{
    if ((iTargetColumn >= 0) && (iTargetColumn < columnCount()))
    {
        // Reset first row to default values
        m_vData[iTargetColumn] = m_lDefaultValues[iTargetColumn];
        emit dataChanged(index(0, iTargetColumn, QModelIndex()), index(0, iTargetColumn, QModelIndex()));

        // Clear others
        for (int i=1; i<m_nRows+1; i++)
        {
            QModelIndex targetIndex = index(i, iTargetColumn, QModelIndex());
            if (targetIndex.isValid())
                setData(targetIndex, QString(""), Qt::EditRole);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::setValue(const QString &sParameterVariable, const QString &sVariableValue)
{
    if (m_hHashTable.contains(sParameterVariable))
    {
        Position p = m_hHashTable[sParameterVariable];
        QModelIndex targetIndex = index(p.row, p.column, QModelIndex());
        if (targetIndex.isValid())
        {
            QString sMsg = QString("SETTING VALUE: %1 FOR VARIABLE: %2").arg(sVariableValue).arg(sParameterVariable);
            logInfo(sMsg);
            setData(targetIndex, sVariableValue, Qt::EditRole);
        }
    }
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::evaluateAutoScript(const QString &sFullScript)
{
    QStringList lSingleScripts = sFullScript.split(";");
    foreach (QString sSingleScript, lSingleScripts)
        evaluateSingleScript(sSingleScript);
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::evaluateSingleScript(const QString &sSingleScript)
{
    QStringList lSplitted = sSingleScript.split("=");
    if (lSplitted.size() == 2)
    {
        QString sColumnVariable = lSplitted.first().simplified();
        QString sLogicalExpression = lSplitted[1].simplified();

        // Identify column index
        int iTargetColumn = m_lColumnVariables.indexOf(sColumnVariable);
        if (iTargetColumn >= 0)
        {
            // Retrieve variable names
            QVector<QString> vVariableNames = m_pController->parameterMgr()->extractVariableNames(sSingleScript);
            QString sMatchedScript = sLogicalExpression;
            bool bSuccess = true;
            foreach (QString sParameterVariable, vVariableNames)
            {
                Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sParameterVariable);
                if (pParameter == nullptr)
                {
                    QString sMsg = QString("CAN'T EVALUATE: %1 SINCE VARIABLE: %2 DOES NOT EXIST").arg(sSingleScript).arg(sParameterVariable);
                    logError(sMsg);
                    bSuccess = false;
                    break;
                }
                sMatchedScript.replace(sParameterVariable, pParameter->value());
            }
            if (bSuccess)
            {
                // Indexed to row
                if (sMatchedScript.contains(PROPERTY_CURRENT_ROW))
                {
                    for (int i=0; i<m_nRows; i++)
                    {
                        QString sIndexedScript = sMatchedScript;
                        sIndexedScript.replace(PROPERTY_CURRENT_ROW, QString::number(i));
                        QScriptEngine expression;
                        QScriptValue xResult = expression.evaluate(sIndexedScript);
                        if (xResult.isNumber())
                            setData(index(i+1, iTargetColumn, QModelIndex()), xResult.toString(), Qt::EditRole);
                    }
                }
                // Not indexed to row
                else
                {
                    for (int i=0; i<m_nRows; i++)
                    {
                        QScriptEngine expression;
                        QScriptValue xResult = expression.evaluate(sMatchedScript);
                        if (xResult.isNumber())
                            setData(index(i+1, iTargetColumn, QModelIndex()), xResult.toString(), Qt::EditRole);
                    }
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------

QString GenericParameterTableModel::getFormattedVariableName(const QString &sVariableMethod, const QString &sTargetVariable, const QStringList &lColumnVariables, const QString &sTargetRow, int iColumn, int iRow)
{
    QString sFormattedVariable("");
    if (sVariableMethod == PROPERTY_VARIABLE_METHOD1)
    {
        sFormattedVariable = ParameterMgr::identifyTargetVariable_method1(sTargetVariable, lColumnVariables, sTargetRow, iColumn, iRow);
    }
    else
    if (sVariableMethod == PROPERTY_VARIABLE_METHOD2)
    {
        sFormattedVariable = ParameterMgr::identifyTargetVariable_method2(sTargetVariable, iRow);
    }
    return sFormattedVariable;
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::processActionSetNumberOfRows(const QString &sActionSetNumberOfRows)
{
    // Retrieve parameter
    Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sActionSetNumberOfRows);
    if (pParameter != nullptr)
    {
        connect(pParameter, &Parameter::parameterValueChanged, this, &GenericParameterTableModel::onSetRowCount, Qt::UniqueConnection);
    }
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::onSetRowCount(const QString &sParameterName, const QString &sParameterValue)
{
    Q_UNUSED(sParameterName);
    bool bOK = true;
    int nRows = sParameterValue.toInt(&bOK);
    if (bOK)
    {
        // Make copy of current
        QVector<QString> vData = m_vData;
        beginResetModel();
        m_nRows = nRows;
        m_vData.resize((nRows+1)*m_lColumnLabels.size());
        clearAll();

        // Write own data
        int iDataSize = qMin(vData.size(), m_vData.size());
        for (int i=0; i<iDataSize; i++)
            m_vData[i] = vData[i];

        // Update corresponding variables
        int nColumns = m_lColumnVariables.size();
        for (int i=nRows; i<m_nMaxNumberOfRows; i++)
        {
            for (int j=0; j<nColumns; j++)
            {
                QString sFormattedVariableName = getFormattedVariableName(m_sVariableMethod, m_sTargetVariable, m_lColumnVariables, m_sTargetRow, j, i);
                QString sMsg = QString("CLEARING: %1").arg(sFormattedVariableName);
                logInfo(sMsg);
                emit parameterValueChanged(sFormattedVariableName, QString(""));
            }
        }

        endResetModel();
        emit rowCountChanged(m_nRows);
    }
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::onUpdateAll(int iTargetColumn)
{
    for (int i=0; i<m_nRows; i++)
    {
        QModelIndex targetIndex = index(i+1, iTargetColumn, QModelIndex());
        if (targetIndex.isValid())
            setData(targetIndex, m_vData[iTargetColumn], Qt::EditRole);
    }
}

//-------------------------------------------------------------------------------------------------

ItemDelegate::ItemDelegate(QObject *parent) : QItemDelegate(parent)
{
}

//-------------------------------------------------------------------------------------------------

ItemDelegate::~ItemDelegate()
{
}

//-------------------------------------------------------------------------------------------------

QWidget *ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QLineEdit *pEditor = new QLineEdit(parent);
    pEditor->setValidator(new DoubleValidator(-1000., 1000., 3));
    return pEditor;
}

//-------------------------------------------------------------------------------------------------

void ItemDelegate::setEditorData(QWidget *pEditor, const QModelIndex &index) const
{
    QString sValue = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *pLineEdit = static_cast<QLineEdit *>(pEditor);
    pLineEdit->setText(sValue);
}

//-------------------------------------------------------------------------------------------------


void ItemDelegate::setModelData(QWidget *pEditor, QAbstractItemModel *pModel, const QModelIndex &index) const
{
    QLineEdit *pLineEdit = static_cast<QLineEdit *>(pEditor);
    QString sValue = pLineEdit->text();
    pModel->setData(index, sValue);
}

//-------------------------------------------------------------------------------------------------

void ItemDelegate::updateEditorGeometry(QWidget *pEditor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    pEditor->setGeometry(option.rect);
}

//-------------------------------------------------------------------------------------------------

GenericParameterTable::GenericParameterTable(Controller *pController, const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QString &sDefaultValue, const QString &sTargetRow,
                                             int nRows, const QString &sTargetVariable,  const QString &sVariableMethod, const QString &sActionSetNumberOfRows, const QString &sAutoScript, QWidget *parent) :
    BaseWidget(pController, parent),  ui(new Ui::GenericParameterTable)
{
    // Setup UI
    ui->setupUi(this);
    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Set default value
    setDefaultValue(sDefaultValue);

    // Set auto script
    setAutoScript(sAutoScript);

    // Build item delegate
    ItemDelegate *pItemDelegate = new  ItemDelegate;
    ui->tableView->setItemDelegate(pItemDelegate);

    // Stretch columns
    CustomHeaderView *pHeaderView = new CustomHeaderView(lColumnLabels.toVector());
    ui->tableView->setHorizontalHeader(pHeaderView);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set model
    m_pModel = new GenericParameterTableModel(m_pController, lColumnLabels, lColumnVariables, defaultValue(), sTargetRow, nRows, sTargetVariable, sVariableMethod, sActionSetNumberOfRows, this);
    ui->tableView->setModel(m_pModel);
    ui->tableView->onRowCountChanged();
    connect(m_pModel, &GenericParameterTableModel::rowCountChanged, ui->tableView, &CustomTableView::onRowCountChanged);
    connect(m_pModel, &GenericParameterTableModel::parameterValueChanged, this, &GenericParameterTable::parameterValueChanged, Qt::UniqueConnection);

    // Populate button area
    connect(pHeaderView, &CustomHeaderView::clearClicked, this, &GenericParameterTable::onClearColumn, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------

GenericParameterTable::~GenericParameterTable()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

QHash<QString, Position> GenericParameterTable::parameterVariableHashTable() const
{
    return m_pModel->parameterVariableHashTable();
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTable::applyDefaultValue()
{
    applyValue(defaultValue());
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTable::clearAll()
{
    m_pModel->clearAll();
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTable::applyValue(const QString &sValue)
{
    m_pModel->applyValue(sValue);
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTable::setValue(const QString &sParameterVariable, const QString &sVariableValue)
{
    m_pModel->setValue(sParameterVariable, sVariableValue);
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTable::onEvaluateAutoScript()
{
    m_pModel->evaluateAutoScript(autoScript());
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTable::onClearColumn(int iTargetColumn)
{
    m_pModel->clearColumn(iTargetColumn);
}


