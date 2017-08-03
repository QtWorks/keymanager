#ifndef EXCLUSIVECHOICEWIDGET_H
#define EXCLUSIVECHOICEWIDGET_H

// Qt
#include <QWidget>

// Application
class QRadioButton;
namespace Ui {
class ExclusiveChoiceWidget;
}

class ExclusiveChoiceWidget : public QWidget
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
    ~ExclusiveChoiceWidget();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Setup
    void setup(const QString &sLabel, const QStringList &lLabels, const QStringList &lValues);

    //! Apply default value
    void applyDefaultValue();

private:
    //! UI
    Ui::ExclusiveChoiceWidget *ui;

    //! Radio buttons
    QVector<QRadioButton *> m_vRadioButtons;

    //! Default value
    QString m_sDefaultValue;

public slots:
    //! Radio button clicked
    void onRadioButtonClicked(bool bClicked);

signals:
    //! Selection changed
    void selectionChanged(const QString &sSelection);
};

#endif // EXCLUSIVECHOICEWIDGET_H
