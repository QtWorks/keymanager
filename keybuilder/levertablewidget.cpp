// Qt
#include <QDebug>
#include <QPushButton>

// Application
#include "levertablewidget.h"
#include "ui_levertablewidget.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

LeverTableModel::LeverTableModel(const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QString &sTargetRow,
                                 int nRows, const QString &sTargetVariable,  QObject *parent) : QAbstractItemModel(parent)
{
    int nColumns = qMin(lColumnLabels.size(), lColumnVariables.size());
    m_lColumnLabels = lColumnLabels.mid(0, nColumns);
    m_lColumnVariables = lColumnVariables.mid(0, nColumns);
    m_sTargetRow = sTargetRow;
    m_nRows = nRows;
    m_nActiveLever = nRows;
    m_nEnabledRows = nRows;
    m_sTargetVariable = sTargetVariable;
    m_vData.resize(nRows*nColumns);
}

//-------------------------------------------------------------------------------------------------

LeverTableModel::~LeverTableModel()
{

}

//-------------------------------------------------------------------------------------------------

const QStringList &LeverTableModel::columnLabels() const
{
    return m_lColumnLabels;
}

//-------------------------------------------------------------------------------------------------

const QStringList &LeverTableModel::columnVariables() const
{
    return m_lColumnVariables;
}

//-------------------------------------------------------------------------------------------------

QModelIndex LeverTableModel::index(int iRow, int iCol, const QModelIndex &parent) const
{
    if (!parent.isValid())
        return createIndex(iRow, iCol);
    return QModelIndex();
}

//-------------------------------------------------------------------------------------------------

QModelIndex LeverTableModel::parent(const QModelIndex &) const
{
    return QModelIndex();
}

//-------------------------------------------------------------------------------------------------

int LeverTableModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_nRows;
    return 0;
}

//-------------------------------------------------------------------------------------------------

int LeverTableModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_lColumnLabels.size();
    return 0;
}

//-------------------------------------------------------------------------------------------------

QVariant LeverTableModel::data(const QModelIndex &index, int iRole) const
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

bool LeverTableModel::setData(const QModelIndex &index, const QVariant &vData, int iRole)
{
    if (index.isValid())
        if (iRole == Qt::EditRole)
        {
            double dValue = vData.toDouble();
            m_vData[index.column()+index.row()*m_lColumnVariables.size()] = dValue;
            emit parameterValueChanged(identifyTargetVariable(index.column(), index.row()), vData.toString());
            emit dataChanged(index, index, QVector<int>() << Qt::DisplayRole);
            return true;
        }

    return false;
}

//-------------------------------------------------------------------------------------------------

QVariant LeverTableModel::headerData(int section, Qt::Orientation eOrientation, int role) const
{
    if (eOrientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractItemModel::headerData(section, eOrientation, role);
    return m_lColumnLabels[section];
}

//-------------------------------------------------------------------------------------------------

Qt::ItemFlags LeverTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (index.isValid() && (index.row() > (m_nActiveLever-1)))
        flags &= (~Qt::ItemIsEnabled);

    return flags|Qt::ItemIsEditable;
}

//-------------------------------------------------------------------------------------------------

void LeverTableModel::setNumActiveLever(int iNumActiveLever)
{
    beginResetModel();
    m_nActiveLever = iNumActiveLever;
    endResetModel();
}

//-------------------------------------------------------------------------------------------------

void LeverTableModel::resetColumnVariables(int iColumnIndex)
{
    for (int i=0; i<m_nRows; i++)
        setData(index(i, iColumnIndex, QModelIndex()), 0., Qt::EditRole);
}

//-------------------------------------------------------------------------------------------------

QString LeverTableModel::identifyTargetVariable(int iColumn, int iRow) const
{
    QString sRowNumber = QString::number(iRow+1);
    if (sRowNumber.length() < 2)
        sRowNumber = "0"+sRowNumber;
    QString sTargetVariable = QString(m_sTargetVariable).arg(m_sTargetRow).arg(sRowNumber).arg(m_lColumnVariables[iColumn]);
    return sTargetVariable;
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

LeverTableWidget::LeverTableWidget(const QStringList &lColumnLabels, const QStringList &lColumnVariables, const QString &sTargetRow,
                                   int nRows, const QString &sTargetVariable,  QWidget *parent) : QWidget(parent),  ui(new Ui::LeverTableWidget)
{
    // Setup UI
    ui->setupUi(this);

    // Build item delegate
    ItemDelegate *pItemDelegate = new  ItemDelegate;
    ui->tableView->setItemDelegate(pItemDelegate);

    // Set validator
    ui->lineEdit->setValidator(new QIntValidator(5, nRows, this));
    connect(ui->lineEdit, &QLineEdit::textChanged, this, &LeverTableWidget::onNumActiveLeverChanged);

    // Stretch columns
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set model
    m_pModel = new LeverTableModel(lColumnLabels, lColumnVariables, sTargetRow, nRows, sTargetVariable);
    ui->tableView->setModel(m_pModel);
    connect(m_pModel, &LeverTableModel::parameterValueChanged, this, &LeverTableWidget::parameterValueChanged);

    // Populate button area
    populateButtonArea();
}

//-------------------------------------------------------------------------------------------------

LeverTableWidget::~LeverTableWidget()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void LeverTableWidget::populateButtonArea()
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

void LeverTableWidget::onNumActiveLeverChanged()
{
    QLineEdit *pSender = dynamic_cast<QLineEdit *>(sender());
    if (pSender != nullptr)
    {
        m_pModel->setNumActiveLever(pSender->text().toInt());
    }
}

//-------------------------------------------------------------------------------------------------

void LeverTableWidget::onActionButtonClicked()
{
    QPushButton *pSender = dynamic_cast<QPushButton *>(sender());
    if (pSender != nullptr)
    {
        int iTargetColumn = pSender->property(PROPERTY_USER_VALUE).toInt();
        resetColumnVariables(iTargetColumn);
    }
}

//-------------------------------------------------------------------------------------------------

void LeverTableWidget::resetColumnVariables(int iColumnIndex)
{
    m_pModel->resetColumnVariables(iColumnIndex);
}

