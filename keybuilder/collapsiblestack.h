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
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Add panel
    CollapsiblePanel *addPanel(const QString &sCaption, QWidget *pWidget);

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
};

#endif // COLLAPSIBLESTACK_H
