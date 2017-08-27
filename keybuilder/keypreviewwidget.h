#ifndef KEYPREVIEWWIDGET_H
#define KEYPREVIEWWIDGET_H

// Qt
#include <QWidget>
#include <QImage>

// Application
class Controller;
class CollapsibleBlock;

class KeyPreviewWidget : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit KeyPreviewWidget(QWidget *parent=nullptr);

    //! Destructor
    ~KeyPreviewWidget();

    //! Set controller
    void setController(Controller *pController);

    //! Set no cut selected
    void setNoCutSelected(bool bNoCutSelected);

protected:
    //! Handle paint event
    virtual void paintEvent(QPaintEvent *event);

private:
    //! Compute scale factor
    static double computeScaleFactor(const QRect &srcRect, const QRect &dstRect);

private:
    //! Own image
    QImage *m_pKeyImage;

    //! Controller
    Controller *m_pController;

    //! No cut selected?
    bool m_bNoCutSelected;

public slots:
    //! Update key image
    void onUpdateKeyImage();

    //! Block selection status changed
    void onBlockSelectionStatusChanged(CollapsibleBlock *pBlock);
};

#endif // KEYPREVIEWWIDGET_H
