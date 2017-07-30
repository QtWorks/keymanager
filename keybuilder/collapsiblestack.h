#ifndef COLLAPSIBLESTACK_H
#define COLLAPSIBLESTACK_H

// Qt
#include <QWidget>

// Application
class CollapsiblePanel;
class QVBoxLayout;

class CollapsibleStack : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit CollapsibleStack(QWidget *parent=nullptr);

    //! Destructor
    ~CollapsibleStack();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return list of panels
    const QVector<CollapsiblePanel *> &panels() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Add panel
    void addPanel(const QString &sCaption, QWidget *pKeyBlock, bool bHasParameters);

    //! Expand all
    void expandAll();

    //! Collapse all
    void collapseAll();

private:
    //! Return all collapsed state
    bool allCollapsed() const;

    //! Own layout
    QVBoxLayout *m_pLayout;

    //! Panels
    QVector<CollapsiblePanel *> m_vPanels;

public slots:
    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

    //! Panel selected
    void onPanelSelected();

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Panel selected
    void panelSelected(CollapsiblePanel *pPanel);
};

#endif // COLLAPSIBLESTACK_H
