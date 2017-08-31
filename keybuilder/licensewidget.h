#ifndef LICENSEWIDGET_H
#define LICENSEWIDGET_H

// Qt
#include <QWidget>

namespace Ui {
class LicenseWidget;
}

class LicenseWidget : public QWidget
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

    //! Set question
    void setQuestion(const QString &sQuestion);

private:
    //! UI
    Ui::LicenseWidget *ui;

public slots:
    //! Validate clicked
    void onValidateClicked();

signals:
    //! Validate clicked
    void validateClicked(const QString &sQuestion, const QString &sAnswer);
};

#endif // LICENSEWIDGET_H
