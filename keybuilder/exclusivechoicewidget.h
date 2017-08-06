#ifndef EXCLUSIVECHOICEWIDGET_H
#define EXCLUSIVECHOICEWIDGET_H

// Application
#include "basewidget.h"
class QRadioButton;
namespace Ui {
class ExclusiveChoiceWidget;
}

class ExclusiveChoiceWidget : public BaseWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit ExclusiveChoiceWidget(QWidget *parent=nullptr);

    //! Constructor
    explicit ExclusiveChoiceWidget(const QStringList &lLabels, const QStringList &lValues, const QString &sLabel, const QString &sDefaultValue, QWidget *parent=nullptr);

    //! Destructor
    virtual ~ExclusiveChoiceWidget();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Setup
    void setup(const QString &sLabel, const QStringList &lLabels, const QStringList &lValues);

    //! Apply default value
    virtual void applyDefaultValue();

private:
    //! UI
    Ui::ExclusiveChoiceWidget *ui;

    //! Radio buttons
    QVector<QRadioButton *> m_vRadioButtons;

public slots:
    //! Radio button clicked
    void onRadioButtonToggled(bool bClicked);

signals:
    //! Selection changed
    void selectionChanged(const QString &sSelection);
};

#endif // EXCLUSIVECHOICEWIDGET_H
