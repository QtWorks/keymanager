// Qt
#include <QDebug>
#include <QPushButton>

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
    m_pController(pController)
{
    QStringList lDefaultValues;
    if (sDefaultValue.contains(","))
        lDefaultValues = sDefaultValue.split(",");

    int nColumns = qMin(lColumnLabels.size(), lColumnVariables.size());
    if (!lDefaultValues.isEmpty())
    {
        nColumns = qMin(nColumns, lDefaultValues.size());
        m_lDefaultValues = lDefaultValues.mid(0, nColumns);
    }
    else
    {
        for (int i=0; i<nColumns; i++)
            m_lDefaultValues << PROPERTY_DEFAULT_VALUE;
    }

    m_lColumnLabels = lColumnLabels.mid(0, nColumns);
    m_lColumnVariables = lColumnVariables.mid(0, nColumns);
    m_sTargetRow = sTargetRow;
    m_nRows = nRows;
    m_nActiveLever = nRows;
    m_nEnabledRows = nRows;
    m_sTargetVariable = sTargetVariable;
    m_sVariableMethod = sVariableMethod;
    m_sActionSetNumberOfPins = sActionSetNumberOfPins;
    if (!m_sActionSetNumberOfPins.isEmpty())
    {
        processActionSetNumberOfPins(m_sActionSetNumberOfPins);
    }
    m_vData.resize(nRows*nColumns);

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
        return m_nRows;
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
            return m_vData[index.column()+index.row()*m_lColumnVariables.size()];
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
        double dValue = vData.toDouble();
        m_vData[index.column()+index.row()*m_lColumnVariables.size()] = dValue;

        QString sFormattedVariable = getFormattedVariableName(m_sVariableMethod, m_sTargetVariable, m_lColumnVariables, m_sTargetRow, index.column(), index.row());
        emit parameterValueChanged(sFormattedVariable, vData.toString());
        emit dataChanged(index, index, QVector<int>() << Qt::DisplayRole);
        return true;

    }

    return false;
}

//-------------------------------------------------------------------------------------------------

QVariant GenericParameterTableModel::headerData(int section, Qt::Orientation eOrientation, int role) const
{
    if (eOrientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractItemModel::headerData(section, eOrientation, role);
    return m_lColumnLabels[section];
}

//-------------------------------------------------------------------------------------------------

Qt::ItemFlags GenericParameterTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (index.isValid() && (index.row() > (m_nActiveLever-1)))
        flags &= (~Qt::ItemIsEnabled);

    return flags|Qt::ItemIsEditable;
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::setNumActiveLever(int iNumActiveLever)
{
    beginResetModel();
    m_nActiveLever = iNumActiveLever;
    endResetModel();
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::resetColumnVariables(int iColumnIndex)
{
    for (int i=0; i<m_nRows; i++)
        setData(index(i, iColumnIndex, QModelIndex()), "0", Qt::EditRole);
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTableModel::applyValue(const QString &sValue)
{
    QStringList lValues;
    if (sValue.contains(","))
        lValues = sValue.split(",");

    if (!lValues.isEmpty())
    {
        for (int i=0; i<m_nRows; i++)
        {
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
    qDebug() << "NEED TO SET NUMBER OF ROWS TO: " << sParameterValue;
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

    // Set validator
    ui->lineEdit->setValidator(new IntValidator(5, nRows, this));
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &GenericParameterTable::onNumActiveLeverChanged, Qt::UniqueConnection);

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

void GenericParameterTable::onNumActiveLeverChanged()
{
    QLineEdit *pSender = dynamic_cast<QLineEdit *>(sender());
    if (pSender != nullptr)
    {
        m_pModel->setNumActiveLever(pSender->text().toInt());
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

