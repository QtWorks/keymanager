#ifndef HEADEROBJECT_H
#define HEADEROBJECT_H

#include <QWidget>

namespace Ui {
class HeaderObject;
}

class HeaderObject : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit HeaderObject(QWidget *parent=nullptr);

    //! Constructor
    HeaderObject(int iColumnIndex, const QString &sLabel, QWidget *parent=nullptr);

    //! Destructor
    ~HeaderObject();

private:
    //! UI
    Ui::HeaderObject *ui;

    //! Column index
    int m_iColumnIndex;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Button clicked
    void onButtonClicked();

signals:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Button clicked
    void clearClicked(int iColumnIndex);
};

#endif // HEADEROBJECT_H
