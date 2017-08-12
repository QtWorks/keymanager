// Qt
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>

// Application
#include "genericparametertable.h"
#include "ui_genericparametertable.h"
#include "controller.h"
#include "parametermgr.h"
#include "constants.h"
#include "intvalidator.h"

//-------------------------------------------------------------------------------------------------

GenericParameterTableModel::GenericParameterTableModel(Controller *pController, const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QString &sDefaultValue, const QString &sTargetRow,
    int nRows, const QString &sTargetVariable, const QString &sVariableMethod, const QString &sActionSetNumberOfPins, QObject *parent) : QAbstractItemModel(parent),
    m_pController(pController), m_sDefaultValue(sDefaultValue)
{
    if (m_sDefaultValue.simplified().isEmpty())
        m_sDefaultValue = PROPERTY_DEFAULT_VALUE;
    QStringList lDefaultValues;
    if (m_sDefaultValue.contains(","))
        lDefaultValues = m_sDefaultValue.split(",");
    else lDefaultValues << m_sDefaultValue;

    int nColumns = qMin(lColumnLabels.size(), lColumnVariables.size());
    if (!lDefaultValues.isEmpty())
        nColumns = qMin(nColumns, lDefaultValues.size());

    m_lColumnLabels = lColumnLabels.mid(0, nColumns);
    m_lColumnVariables = lColumnVariables.mid(0, nColumns);
    m_sTargetRow = sTargetRow;
    m_nRows = nRows;
    m_nEnabledRows = nRows;
    m_sTargetVariable = sTargetVariable;
    m_sVariableMethod = sVariableMethod;
    m_sActionSetNumberOfPins = sActionSetNumberOfPins;
    if (!m_sActionSetNumberOfPins.isEmpty())
    {
        processActionSetNumberOfPins(m_sActionSetNumberOfPins);
    }
    m_vData.resize(nRows*nColumns);
    for (int i=0; i<lDefaultValues.size(); i++)
        m_vAllDataRow << lDefaultValues[i];
    for (int i=0; i<nRows; i++)
    {
        for (int j=0; j<nColumns; j++)
        {
            // Build formatted variable name
            QString sFormattedVariableName = getFormattedVariableName(m_sVariableMethod, sTargetVariable, m_lColumnVariables, m_sTargetRow, j, i);
            Position p;
            p.column = j;
            p.row = i;
            m_hHashTable[sFormattedVariableName] = p;
        }
    }
    connect(this, &GenericParameterTableModel::updateAll, this, &GenericParameterTableModel::onUpdateAll);
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
            int iTargetIndex = index.row()-1;
            if (iTargetIndex >= 0)
                return m_vData[index.column()+iTargetIndex*m_lColumnVariables.size()];
            else
                return m_vAllDataRow[index.column()];
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
        int iTargetIndex = index.row()-1;
        if (iTargetIndex >= 0)
        {
            m_vData[index.column()+iTargetIndex*m_lColumnVariables.size()] = sValue;
            QString sFormattedVariable = getFormattedVariableName(m_sVariableMethod, m_sTargetVariable, m_lColumnVariables, m_sTargetRow, index.column(), iTargetIndex);
            emit parameterValueChanged(sFormattedVariable, vData.toString());
            emit dataChanged(index, index, QVector<int>() << Qt::DisplayRole);
        }
        else
        {
            m_vAllDataRow[index.column()] = sValue;
            emit dataChanged(index, index, QVector<int>() << Qt::DisplayRole);
            emit updateAll(index.column());
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

void GenericParameterTableModel::resetColumnVariables(int iColumnIndex)
{
    applyValue(m_sDefaultValue, iColumnIndex);
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::applyValue(const QString &sValue, int iTargetColumn)
{
    QStringList lValues;
    if (sValue.contains(","))
        lValues = sValue.split(",");
    else lValues << m_sDefaultValue;

    if (!lValues.isEmpty())
    {
        for (int i=0; i<m_nRows; i++)
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

void GenericParameterTableModel::setValue(const QString &sParameterVariable, const QString &sVariableValue)
{
    if (m_hHashTable.contains(sParameterVariable))
    {
        Position p = m_hHashTable[sParameterVariable];
        QModelIndex targetIndex = index(p.row, p.column, QModelIndex());
        if (targetIndex.isValid())
        {
            qDebug() << "-------------------------------------------------------------------> SETTING VALUE: " << sVariableValue << " FOR VARIABLE: " << sParameterVariable;
            setData(targetIndex, sVariableValue, Qt::EditRole);
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

void GenericParameterTableModel::processActionSetNumberOfPins(const QString &sActionSetNumberOfPins)
{
    // Retrieve parameter
    Parameter *pParameter = m_pController->parameterMgr()->getParameterByVariableName(sActionSetNumberOfPins);
    if (pParameter != nullptr)
    {
        connect(pParameter, &Parameter::parameterValueChanged, this, &GenericParameterTableModel::onSetNumberOfRows, Qt::UniqueConnection);
    }
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::onSetNumberOfRows(const QString &sParameterName, const QString &sParameterValue)
{
    Q_UNUSED(sParameterName);
    bool bOK = true;
    int nRows = sParameterValue.toInt(&bOK);
    if (bOK)
    {
        beginResetModel();
        m_nRows = nRows;
        m_vData.resize(nRows*m_lColumnLabels.size());
        applyValue(m_sDefaultValue);
        endResetModel();
    }
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::onUpdateAll(int iColumnIndex)
{
    for (int i=0; i<m_nRows; i++)
    {
        QModelIndex targetIndex = index(i+1, iColumnIndex, QModelIndex());
        if (targetIndex.isValid())
            setData(targetIndex, m_vAllDataRow[iColumnIndex], Qt::EditRole);
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
    pEditor->setValidator(new QDoubleValidator());
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
                                             int nRows, const QString &sTargetVariable,  const QString &sVariableMethod, const QString &sActionSetNumberOfPins, QWidget *parent) :
    BaseWidget(pController, parent),  ui(new Ui::GenericParameterTable)
{
    // Setup UI
    ui->setupUi(this);
    m_sDefaultValue = sDefaultValue;

    // Build item delegate
    ItemDelegate *pItemDelegate = new  ItemDelegate;
    ui->tableView->setItemDelegate(pItemDelegate);

    // Stretch columns
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set model
    m_pModel = new GenericParameterTableModel(m_pController, lColumnLabels, lColumnVariables, m_sDefaultValue, sTargetRow, nRows, sTargetVariable, sVariableMethod, sActionSetNumberOfPins, this);
    ui->tableView->setModel(m_pModel);
    connect(m_pModel, &GenericParameterTableModel::parameterValueChanged, this, &GenericParameterTable::parameterValueChanged, Qt::UniqueConnection);

    // Populate button area
    populateButtonArea();
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
    applyValue(m_sDefaultValue);
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

void GenericParameterTable::populateButtonArea()
{
    QStringList lColumnLabels = m_pModel->columnLabels();
    for (int i=0; i<lColumnLabels.size(); i++)
    {
        QString sButtonLabel = QString("CLEAR ALL %1").arg(lColumnLabels[i]);
        QPushButton *pButton = new QPushButton(sButtonLabel, this);
        pButton->setProperty(PROPERTY_USER_VALUE, i);
        connect(pButton, &QPushButton::clicked, this, &onActionButtonClicked, Qt::UniqueConnection);
        ui->buttonLayout->addWidget(pButton);
    }
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTable::onActionButtonClicked()
{
    QPushButton *pSender = dynamic_cast<QPushButton *>(sender());
    if (pSender != nullptr)
    {
        int iTargetColumn = pSender->property(PROPERTY_USER_VALUE).toInt();
        resetColumnVariables(iTargetColumn);
    }
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTable::resetColumnVariables(int iColumnIndex)
{
    m_pModel->resetColumnVariables(iColumnIndex);
}

