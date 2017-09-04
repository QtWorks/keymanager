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

    //! Set block count
    void setBlockCount(int iBlockCount);

protected:
    //! Event filter
    bool eventFilter(QObject *pWatched, QEvent *event);

private:
    //! Build blocks
    void buildBlocks();

private:
    //! UI
    Ui::KeyWidget *ui;

    //! Blocks
    QVector<QLineEdit *> m_vBlocks;

    //! Block count
    int m_iBlockCount;

public slots:
    //! Copy value to clipboard
    void onCopyValueToClipBoard();
};

#endif // KEYWIDGET_H
