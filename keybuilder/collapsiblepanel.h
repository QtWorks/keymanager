#ifndef COLLAPSIBLEPANEL_H_
#define COLLAPSIBLEPANEL_H_

#include <QWidget>
#include <QLabel>

class QVBoxLayout;

class CaptionLabel : public QLabel
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CaptionLabel(const QString &caption, bool bExpandable, QWidget *parent);

    //! Destructor
    ~CaptionLabel();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Collapse
    void collapse();

    //! Expand
    void expand();

    //! Is collpased
    bool isCollapsed() const { return m_bCollpased; }

    //-------------------------------------------------------------------------------------------------
    // Overloaded methods
    //-------------------------------------------------------------------------------------------------

    //! Handle mouse press events
    void mousePressEvent(QMouseEvent *e) override;

    //! Handle paint events
    void paintEvent(QPaintEvent *event) override;

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Notify a collapse/expand change
    void collapseOrExpand(bool);

private:
    //! Collapsed state
    bool m_bCollpased;

    //! Expandable?
    bool m_bExpandable;
};

class CollapsiblePanel : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    CollapsiblePanel(const QString &caption, bool bHasParameters=true, QWidget *parent=nullptr);

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

    //! Set caption
    void setCaption(const QString &caption);

    //! Return collapsed state
    bool isCollapsed() const;

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

private:
    //! Own widget
    QWidget *m_pWidget;

    //! Own layout
    QVBoxLayout *m_pLayout;

    //! Caption
    CaptionLabel *m_pLabel;

    //! Has parameters?
    bool m_bHasParameters;
};

#endif /*COLLAPSIBLEPANEL_H_*/
