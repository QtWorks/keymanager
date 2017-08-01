#ifndef COLLAPSIBLEBLOCK_H
#define COLLAPSIBLEBLOCK_H

// Qt
#include <QWidget>
#include <QLabel>
class QVBoxLayout;

// Application
class CaptionLabel;

class CollapsibleBlock : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CollapsibleBlock(QWidget *pWidget, const QString &sCaption, bool bIsEmpty, QWidget *parent=nullptr);

    //! Destructor
    ~CollapsibleBlock();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return widget
    QWidget *widget() const;

    //! Is current?
    bool isCurrent() const;

    //! Set current
    void setCurrent(bool bCurrent);

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Return closed state
    bool isClosed() const;

private:
    //! Set widget
    void setWidget(QWidget *widget);

private:
    //! Own widget
    QWidget *m_pWidget;

    //! Own layout
    QVBoxLayout *m_pLayout;

    //! Caption
    CaptionLabel *m_pLabel;

    //! Has parameters or child blocks?
    bool m_bIsEmpty;

    //! Closed?
    bool m_bIsClosed;

    //! Is current?
    bool m_bIsCurrent;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Close or open
    void onClose(bool);

    //! Toggle closed state
    void onToggleClosedState();

signals:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Block selected
    void blockSelected();

    //! State changed (closed or opened)
    void closedStateChanged(bool bClosed);
};

#endif /*COLLAPSIBLEBLOCK_H*/
