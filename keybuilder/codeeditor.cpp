// Application
#include "codeeditor.h"
#include "ui_codeeditor.h"

//-------------------------------------------------------------------------------------------------

CodeEditor::CodeEditor(QWidget *parent) : QWidget(parent),
    ui(new Ui::CodeEditor)
{
    ui->setupUi(this);
    connect(ui->searchArea, &QLineEdit::returnPressed, this, &CodeEditor::onSearchTextChanged);
}

//-------------------------------------------------------------------------------------------------

CodeEditor::~CodeEditor()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------

void CodeEditor::load(const QString &sInputString)
{
    ui->plainTextEdit->load(sInputString);
}

//-------------------------------------------------------------------------------------------------

void CodeEditor::onSearchTextChanged()
{
    ui->plainTextEdit->doSearch(ui->searchArea->text());
}
