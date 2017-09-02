#ifndef KEYWIDGET_H
#define KEYWIDGET_H

// Qt
#include <QWidget>
class QLineEdit;

namespace Ui {
class KeyWidget;
}

class KeyWidget : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit KeyWidget(QWidget *parent=nullptr);

    //! Destructor
    ~KeyWidget();

    //! Set title
    void setTitle(const QString &sTitle);

    //! Set read only
    void setReadOnly(bool bReadOnly);

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return value
    QString getValue() const;

    //! Set value
    void setValue(const QString &sValue);

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Show clipboard button
    void showClipBoardButton(bool bShow);

private:
    //! UI
    Ui::KeyWidget *ui;

    //! Blocks
    QVector<QLineEdit *> m_vBlocks;

public slots:
    //! Copy value to clipboard
    void onCopyValueToClipBoard();
};

#endif // KEYWIDGET_H
