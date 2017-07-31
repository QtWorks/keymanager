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
    CollapsibleBlock(const QString &sCaption, bool bHasParameters, QWidget *parent=nullptr);

    //! Destructor
    ~CollapsibleBlock();

    //-------------------------------------------------------------------------------------------------
    // Getters
    //-------------------------------------------------------------------------------------------------

    //! Return widget
    QWidget *widget() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Set widget
    void setWidget(QWidget *widget);

    //! Return collapsed state
    bool isCollapsed() const;

    //! Set current
    void setCurrent(bool bCurrent);

private:
    //! Own widget
    QWidget *m_pWidget;

    //! Own layout
    QVBoxLayout *m_pLayout;

    //! Caption
    CaptionLabel *m_pLabel;

    //! Has parameters?
    bool m_bHasParameters;

    //! Collapsed?
    bool m_bIsCollapsed;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Collapse or expand
    void onCollapse(bool);

    //! Toggle collapsed state
    void onToggleCollapsedState();

signals:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Block selected
    void blockSelected();

    //! State changed (collapsed or expanded)
    void stateChanged(bool bCollapsed);
};

#endif /*COLLAPSIBLEBLOCK_H*/
