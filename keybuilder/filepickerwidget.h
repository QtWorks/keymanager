#ifndef FILEPICKERWIDGET_H
#define FILEPICKERWIDGET_H

// Application
#include "basewidget.h"
class QLineEdit;

namespace Ui {
class FilePickerWidget;
}

class FilePickerWidget : public BaseWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit FilePickerWidget(const QString &sLabel, const QString &sFileExtension, const QString &sDefaultValue, const QString &sAutoScript, const QString &sEnabledCondition, QWidget *parent=nullptr);

    //! Destructor
    virtual ~FilePickerWidget();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Set label
    void setLabel(const QString &sLabel);

    //! Set extension
    void setExtension(const QString &sExtenion);

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Apply default value
    virtual void applyDefaultValue();

    //! Apply value
    virtual void applyValue(const QString &sValue);

private:
    //! UI
    Ui::FilePickerWidget *ui;

    //! File extension
    QString m_sFileExtension;

public slots:
    //! Open clicked
    void onOpenClicked();

    //! File picker text changed
    void onFilePickerTextChanged();

signals:
    //! Text changed
    void textChanged();
};

#endif // FILEPICKERWIDGET_H
