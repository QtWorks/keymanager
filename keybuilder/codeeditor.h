#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>

namespace Ui {
class CodeEditor;
}

class CodeEditor : public QWidget
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit CodeEditor(QWidget *parent=nullptr);

    //! Destructor
    ~CodeEditor();

    //! Load
    void load(const QString &sInputString);

private:
    //! UI
    Ui::CodeEditor *ui;

public slots:
    //! Search text changed, highlight
    void onSearchTextChanged();
};

#endif // CODEEDITOR_H
