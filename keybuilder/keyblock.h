#ifndef KEYBLOCK_H
#define KEYBLOCK_H

#include <QWidget>

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
    explicit KeyBlock(QWidget *parent=nullptr);

    //! Destructor
    ~KeyBlock();

    //! Add widget
    void addWidget(QWidget *pWidget);

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return name
    const QString &name() const;

    //! Set name
    void setName(const QString &sName);

private:
    //! UI
    Ui::KeyBlock *ui;

    //! Name
    QString m_sName;
};

#endif // KEYBLOCK_H
