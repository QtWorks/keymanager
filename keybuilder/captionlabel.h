#ifndef CAPTIONLABEL_H
#define CAPTIONLABEL_H

#include <QWidget>

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

protected:
    //! Paint event
    void paintEvent(QPaintEvent *e);

    //! Mouse press event
    virtual void mousePressEvent(QMouseEvent *event);

private:
    // UI
    Ui::CaptionLabel *ui;

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

    //! Block selected
    void blockSelected();

    //! Toggle closed state
    void toggleClosedState();

private:
    //! Expandable?
    bool m_bExpandable;

    //! Is current?
    bool m_bIsCurrent;
};

#endif // CAPTIONLABEL_H
