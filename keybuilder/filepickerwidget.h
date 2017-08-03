#ifndef FILEPICKERWIDGET_H
#define FILEPICKERWIDGET_H

// Qt
#include <QWidget>

// Application
class QLineEdit;

namespace Ui {
class FilePickerWidget;
}

class FilePickerWidget : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit FilePickerWidget(QWidget *parent=nullptr);

    //! Constructor
    explicit FilePickerWidget(const QString &sLabel, const QString &sFileExtension, QWidget *parent=nullptr);

    //! Destructor
    ~FilePickerWidget();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return value
    QString value() const;

    //! Set label
    void setLabel(const QString &sLabel);

    //! Set extension
    void setExtension(const QString &sExtenion);

private:
    //! UI
    Ui::FilePickerWidget *ui;

private:
    //! File extension
    QString m_sFileExtension;

public slots:
    //! Open clicked
    void onOpenClicked();

signals:
    //! Text changed
    void textChanged();
};

#endif // FILEPICKERWIDGET_H
