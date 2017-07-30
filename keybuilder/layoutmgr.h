#ifndef LAYOUTMGR_H
#define LAYOUTMGR_H

// Qt
#include <QWidget>

// Application
#include "cxmlnode.h"
namespace Ui {
class LayoutMgr;
}
class KeyBlock;
class CollapsibleStack;

class LayoutMgr : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit LayoutMgr(QWidget *parent=nullptr);

    //! Destructor
    ~LayoutMgr();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Add key block
    void addKeyBlock(const CXMLNode &xKeyBlock);

private:
    //! Create key block
    KeyBlock *createKeyBlock(const CXMLNode &xKeyBlock, QString &sBlockName);

    //! Find associated parameter variable
    QString findAssociatedParameterVariable(QWidget *pWidget) const;

private:
    //! UI
    Ui::LayoutMgr *ui;

    //! Number of key blocks
    int m_nKeyBlocks;

    //! Vector of stacks
    QVector<CollapsibleStack *> m_vStacks;

    //! Widget hash
    QHash<QString, QVector<QWidget *> > m_hWidgetHash;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! A line edit text changed
    void onLineEditTextChanged();

    //! A radio button was clicked
    void onRadioButtonClicked();

    //! Close all key blocks in menu 1
    void onExpandAll();

    //! Open all key blocks in menu 1
    void onCollapseAll();

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);
};

#endif // LAYOUTMGR_H
