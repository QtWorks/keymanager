#ifndef PARAMETERBLOCK_H
#define PARAMETERBLOCK_H

// Qt
#include <QWidget>

// Application
#include "cxmlnode.h"
class LayoutMgr;

namespace Ui {
class ParameterBlock;
}

class ParameterBlock : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit ParameterBlock(const CXMLNode &xParameterBlock, LayoutMgr *pLayoutMgr, QWidget *parent=nullptr);

    //! Destructor
    ~ParameterBlock();

    //! Add widget
    void addWidget(QWidget *pWidget, const QString &sParameterVariable);

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return name
    const QString &name() const;

    //! Set name
    void setName(const QString &sName);

    //! Is empty?
    bool isEmpty() const;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! A line edit text changed
    void onLineEditTextChanged();

    //! A radio button was clicked
    void onRadioButtonClicked();

    //! A line edit triplet value changed
    void onLineEditTripletValueChanged();

    //! Select me
    void onSelectMe();

private:
    //! Find associated parameter variable
    QString findAssociatedParameterVariable(QWidget *pWidget) const;

    //! Populate parameter block
    void populateParameterBlock(const CXMLNode &xParameterBlock);

    //! Add widget
    void addWidget(QWidget *pWidget);

private:
    //! UI
    Ui::ParameterBlock *ui;

    //! Name
    QString m_sName;

    //! Widget hash
    QHash<QString, QVector<QWidget *> > m_hWidgetHash;

    //! Is empty?
    bool m_bIsEmpty;

    //! Layout mgr
    LayoutMgr *m_pLayoutMgr;

signals:
    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);
};

#endif // PARAMETERBLOCK_H
