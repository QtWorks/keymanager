#ifndef CAPTIONLABEL_H
#define CAPTIONLABEL_H

// Qt
#include <QWidget>

// Application
class CollapsibleBlock;
namespace Ui {
class CaptionLabel;
}

class CaptionLabel : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit CaptionLabel(QWidget *parent=nullptr);

    //! Destructor
    ~CaptionLabel();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Set state
    void toggleState();

    //! Set current
    void setCurrent(bool bCurrent);

    //! Set caption
    void setCaption(const QString &sCaption);

    //! Set button label
    void setButtonLabel(const QString &sButtonLabel);

    //! Set expandable
    void setExpandable(bool bExpandable);

    //! Update enabled state
    void updateEnabledState(bool bEnabled);

    //! Set block
    void setBlock(CollapsibleBlock *pBlock);

protected:
    //! Paint event
    void paintEvent(QPaintEvent *e);

    //! Mouse press event
    virtual void mousePressEvent(QMouseEvent *event);

public slots:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! State changed
    void onStateChanged(bool bIsClosed);

signals:
    //-------------------------------------------------------------------------------------------------
    // Signals
    //-------------------------------------------------------------------------------------------------

    //! Toggle closed state
    void toggleClosedState();

    //! Clear all
    void clearAll();

private:
    //! UI
    Ui::CaptionLabel *ui;

    //! Enabled?
    bool m_bIsEnabled;

    //! Expandable?
    bool m_bExpandable;

    //! Is current?
    bool m_bIsCurrent;

    //! Block
    CollapsibleBlock *m_pBlock;
};

#endif // CAPTIONLABEL_H
