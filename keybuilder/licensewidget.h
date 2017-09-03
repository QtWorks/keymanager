#ifndef LICENSEWIDGET_H
#define LICENSEWIDGET_H

// Application
#include "descriptiontaggedwidget.h"

namespace Ui {
class LicenseWidget;
}

class LicenseWidget : public DescriptionTaggedWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit LicenseWidget(QWidget *parent=nullptr);

    //! Destructor
    ~LicenseWidget();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Set question
    void setQuestion(const QString &sQuestion);

    //! Set answer
    void setAnswer(const QString &sAnswer);

private:
    //! UI
    Ui::LicenseWidget *ui;

public slots:
    //! Validate clicked
    void onValidateClicked();

signals:
    //! Validate clicked
    void validateClicked(const QString &sAnswer);
};

#endif // LICENSEWIDGET_H
