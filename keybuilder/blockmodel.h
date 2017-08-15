#ifndef BLOCKMODEL_H
#define BLOCKMODEL_H

// Qt
#include <QAbstractItemModel>
#include <QModelIndex>

// Application
class Controller;
class CollapsibleBlock;

class BlockModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit BlockModel(QObject *parent = 0);

    //! Destructor
    ~BlockModel();

    //! Set controller
    void setController(Controller *pController);

    //! Get block
    CollapsibleBlock *getBlock(const QModelIndex &index) const;

    //! Data
    virtual QVariant data(const QModelIndex &index, int role) const;

    //! Return flags
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

    //! Index
    virtual QModelIndex index(int iRow, int iColumn, const QModelIndex &parent=QModelIndex()) const;

    //! Parent
    virtual QModelIndex parent(const QModelIndex &index) const;

    //! Row count
    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;

    //! Column count
    virtual int columnCount(const QModelIndex &parent=QModelIndex()) const;

    // Set root block
    void setRootBlock(CollapsibleBlock *pRootBlock);

protected:
    //! Controller
    Controller *m_pController;

    //: Root block
    CollapsibleBlock *m_pRootBlock;
};

#endif // BLOCKMODEL_H
