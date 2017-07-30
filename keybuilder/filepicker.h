#ifndef FILEPICKER_H
#define FILEPICKER_H

// Qt
#include <QWidget>

// Application
class QLineEdit;

namespace Ui {
class FilePicker;
}

class FilePicker : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit FilePicker(const QString &sFileExtension, QWidget *parent = 0);

    //! Destructor
    ~FilePicker();

    //! Return QLineEdit widget
    QLineEdit *fileLineEdit() const;

private:
    //! UI
    Ui::FilePicker *ui;

public slots:
    //! Open clicked
    void onOpenClicked();

private:
    //! File extension
    QString m_sFileExtension;
};

#endif // FILEPICKER_H
