#ifndef KEYBLOCK_H
#define KEYBLOCK_H

// Qt
#include <QWidget>

// Application
#include "cxmlnode.h"

namespace Ui {
class KeyBlock;
}

class KeyBlock : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit KeyBlock(const CXMLNode &xKeyBlock, QWidget *parent=nullptr);

    //! Destructor
    ~KeyBlock();

    //! Add widget
    void addWidget(QWidget *pWidget, const QString &sParameterVariable);

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return name
    const QString &name() const;

    //! Set name
    void setName(const QString &sName);

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! A line edit text changed
    void onLineEditTextChanged();

    //! A radio button was clicked
    void onRadioButtonClicked();

private:
    //! Find associated parameter variable
    QString findAssociatedParameterVariable(QWidget *pWidget) const;

    //! Populate key block
    void populateKeyBlock(const CXMLNode &xKeyBlock);

    //! Add widget
    void addWidget(QWidget *pWidget);

private:
    //! UI
    Ui::KeyBlock *ui;

    //! Name
    QString m_sName;

    //! Widget hash
    QHash<QString, QVector<QWidget *> > m_hWidgetHash;

signals:
    //! Parameter value changed
    void parameterValueChanged(const QString &sParameterName, const QString &sParameterValue);
};

#endif // KEYBLOCK_H
