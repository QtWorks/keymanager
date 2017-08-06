// Qt
#include <QDebug>
#include <QPushButton>

// Application
#include "genericparametertable.h"
#include "ui_genericparametertable.h"
#include "parametermgr.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

GenericParameterTableModel::GenericParameterTableModel(const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QStringList &lDefaultValues, const QString &sTargetRow,
                                 int nRows, const QString &sTargetVariable,  const QString &sVariableMethod, QObject *parent) : QAbstractItemModel(parent)
{
    int nColumns = qMin(lColumnLabels.size(), lColumnVariables.size());
    if (!lDefaultValues.isEmpty())
        nColumns = qMin(nColumns, lDefaultValues.size());
    m_lColumnLabels = lColumnLabels.mid(0, nColumns);
    m_lColumnVariables = lColumnVariables.mid(0, nColumns);
    if (!lDefaultValues.isEmpty())
        m_lDefaultValues = lDefaultValues.mid(0, nColumns);
    else
    {
        for (int i=0; i<nColumns; i++)
            m_lDefaultValues << "0";
    }
    m_sTargetRow = sTargetRow;
    m_nRows = nRows;
    m_nActiveLever = nRows;
    m_nEnabledRows = nRows;
    m_sTargetVariable = sTargetVariable;
    m_sVariableMethod = sVariableMethod;
    m_vData.resize(nRows*nColumns);
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
    if (index.isValid())
        if (iRole == Qt::EditRole)
        {
            double dValue = vData.toDouble();
            m_vData[index.column()+index.row()*m_lColumnVariables.size()] = dValue;

            QString sFormattedVariable("");
            if (m_sVariableMethod == PROPERTY_VARIABLE_METHOD1)
            {
                sFormattedVariable = ParameterMgr::identifyTargetVariable_method1(m_sTargetVariable, m_lColumnVariables, m_sTargetRow, index.column(), index.row());
                emit parameterValueChanged(sFormattedVariable, vData.toString());
                emit dataChanged(index, index, QVector<int>() << Qt::DisplayRole);
                return true;
            }
            else
            if (m_sVariableMethod == PROPERTY_VARIABLE_METHOD2)
            {
                sFormattedVariable = ParameterMgr::identifyTargetVariable_method2(m_sTargetVariable, index.row());
                emit parameterValueChanged(sFormattedVariable, vData.toString());
                emit dataChanged(index, index, QVector<int>() << Qt::DisplayRole);
                return true;
            }
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

void GenericParameterTableModel::applyDefaultValue()
{
    if (!m_lDefaultValues.isEmpty())
    {
        for (int i=0; i<m_nRows; i++)
        {
            for (int j=0; j<m_lDefaultValues.size(); j++)
                setData(index(i, j, QModelIndex()), m_lDefaultValues[j], Qt::EditRole);
        }
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
    pEditor->setValidator(new QDoubleValidator);
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

GenericParameterTable::GenericParameterTable(const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QStringList &lDefaultValues, const QString &sTargetRow,
                                   int nRows, const QString &sTargetVariable,  const QString &sVariableMethod, QWidget *parent) : BaseWidget(parent),  ui(new Ui::GenericParameterTable)
{
    // Setup UI
    ui->setupUi(this);

    // Build item delegate
    ItemDelegate *pItemDelegate = new  ItemDelegate;
    ui->tableView->setItemDelegate(pItemDelegate);

    // Set validator
    ui->lineEdit->setValidator(new QIntValidator(5, nRows, this));
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &GenericParameterTable::onNumActiveLeverChanged);

    // Stretch columns
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set model
    m_pModel = new GenericParameterTableModel(lColumnLabels, lColumnVariables, lDefaultValues, sTargetRow, nRows, sTargetVariable, sVariableMethod, this);
    ui->tableView->setModel(m_pModel);
    connect(m_pModel, &GenericParameterTableModel::parameterValueChanged, this, &GenericParameterTable::parameterValueChanged);

    // Populate button area
    populateButtonArea();
}

//-------------------------------------------------------------------------------------------------

GenericParameterTable::~GenericParameterTable()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void GenericParameterTable::applyDefaultValue()
{
    m_pModel->applyDefaultValue();
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
        connect(pButton, &QPushButton::clicked, this, &onActionButtonClicked);
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

