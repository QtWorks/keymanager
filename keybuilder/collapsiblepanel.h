#ifndef COLLAPSIBLEPANEL_H_
#define COLLAPSIBLEPANEL_H_

// Qt
#include <QWidget>
#include <QLabel>
class QVBoxLayout;

// Application
class CaptionLabel;

class CollapsiblePanel : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CollapsiblePanel(const QString &sCaption, bool bHasParameters=true, QWidget *parent=nullptr);

    //! Destructor
    ~CollapsiblePanel();

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

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Do collapse
    void collapse();

    //! Do expand
    void expand();

    //! Collapse or expand
    void collapseOrExpand(bool);

signals:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Panel selected
    void panelSelected();
};

#endif /*COLLAPSIBLEPANEL_H_*/
