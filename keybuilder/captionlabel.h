#ifndef CAPTIONLABEL_H
#define CAPTIONLABEL_H

// Qt
#include <QWidget>

// Application
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

    //! Open/Close
    void openOrClose();

    //! Clear all
    void clearAll();

    //! Select me
    void selectMe();

public slots:
    //! Open or close
    void onOpenOrClose();

    //! Clear all
    void onClearAll();

private:
    //! UI
    Ui::CaptionLabel *ui;

    //! Enabled?
    bool m_bIsEnabled;

    //! Expandable?
    bool m_bExpandable;

    //! Is current?
    bool m_bIsCurrent;
};

#endif // CAPTIONLABEL_H
