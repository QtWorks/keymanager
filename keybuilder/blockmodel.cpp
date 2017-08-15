// Application
#include "blockmodel.h"
#include "collapsibleblock.h"
#include "parameterblock.h"

//-------------------------------------------------------------------------------------------------

BlockModel::BlockModel(QObject *parent) : QAbstractItemModel(parent), m_pController(nullptr),
    m_pRootBlock(nullptr)
{
}

//-------------------------------------------------------------------------------------------------

BlockModel::~BlockModel()
{
}

//-------------------------------------------------------------------------------------------------

void BlockModel::setController(Controller *controller)
{
    m_pController = controller;
}

//-------------------------------------------------------------------------------------------------

CollapsibleBlock *BlockModel::getBlock(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return static_cast<CollapsibleBlock *>(index.internalPointer());
}

//-------------------------------------------------------------------------------------------------

QVariant BlockModel::data(const QModelIndex &index, int role) const
{
    // Check index:
    if (!index.isValid())
        return QVariant();

    // Get node:
    CollapsibleBlock *pBlock = getBlock(index);
    if (!pBlock)
        return QVariant();

    // Node base name:
    if (role == Qt::DisplayRole)
    {
        ParameterBlock *pParameterBlock = pBlock->parameterBlock();
        return (pParameterBlock != nullptr) ? pParameterBlock->name() : QVariant();
    }

    return QVariant();
}

//-------------------------------------------------------------------------------------------------

Qt::ItemFlags BlockModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index);
}

//-------------------------------------------------------------------------------------------------

QModelIndex BlockModel::index(int iRow, int iColumn, const QModelIndex &parent) const
{
    if (!hasIndex(iRow, iColumn, parent))
        return QModelIndex();

    CollapsibleBlock *pParentBlock = !parent.isValid() ? m_pRootBlock : static_cast<CollapsibleBlock *>(parent.internalPointer());
    CollapsibleBlock *pChildBlock = pParentBlock ? pParentBlock->childBlock(iRow) : 0;
    return pChildBlock ? createIndex(iRow, iColumn, pChildBlock) : QModelIndex();
}

//-------------------------------------------------------------------------------------------------

QModelIndex BlockModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CollapsibleBlock *pChildBlock = static_cast<CollapsibleBlock *>(index.internalPointer());
    if (!pChildBlock)
        return QModelIndex();
    CollapsibleBlock *pParentBlock = pChildBlock->parentBlock();

    return pParentBlock == m_pRootBlock ? QModelIndex() : createIndex(pParentBlock->row(), 0, pParentBlock);
}

//-------------------------------------------------------------------------------------------------

int BlockModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    CollapsibleBlock *pParentBlock = !parent.isValid() ? m_pRootBlock : static_cast<CollapsibleBlock *>(parent.internalPointer());

    return pParentBlock ? pParentBlock->childBlocks().size() : 0;
}

//-------------------------------------------------------------------------------------------------

int BlockModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

//-------------------------------------------------------------------------------------------------

void BlockModel::setRootBlock(CollapsibleBlock *pRootBlock)
{
    if (pRootBlock == m_pRootBlock)
        return;
    beginResetModel();
    m_pRootBlock = pRootBlock;
    endResetModel();
}
