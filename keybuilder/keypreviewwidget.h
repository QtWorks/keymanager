#ifndef KEYPREVIEWWIDGET_H
#define KEYPREVIEWWIDGET_H

// Qt
#include <QWidget>
#include <QImage>

// Application
class Controller;

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

public slots:
    //! Update key image
    void onUpdateKeyImage();
};

#endif // KEYPREVIEWWIDGET_H
