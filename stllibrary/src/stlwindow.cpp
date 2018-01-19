#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include "stlwindow.h"
#include "canvas.h"
#include "loader.h"

STLWindow::STLWindow(QWidget *parent) :
    QMainWindow(parent),
    open_action(new QAction("Open...", this)),
    saveas_action(new QAction("Save As...", this)),
    perspective_action(new QAction("Perspective", this)),
    orthogonal_action(new QAction("Orthographic", this)),
    watcher(new QFileSystemWatcher(this))

{
    setWindowTitle("fstl");
    setAcceptDrops(true);

    QGLFormat format;
    format.setVersion(2, 1);
    format.setSampleBuffers(true);

    canvas = new Canvas(format, this);
    setCentralWidget(canvas);

    QObject::connect(watcher, &QFileSystemWatcher::fileChanged,
                     this, &STLWindow::on_watched_change);

    open_action->setShortcut(QKeySequence("CTRL+O"));
    QObject::connect(open_action, &QAction::triggered,
                     this, &STLWindow::on_open);
    saveas_action->setShortcut(QKeySequence("CTRL+S"));
    QObject::connect(saveas_action, &QAction::triggered,
                     this, &STLWindow::on_saveas);

    auto file_menu = menuBar()->addMenu("File");
    file_menu->addAction(open_action);
    file_menu->addAction(saveas_action);

    auto view_menu = menuBar()->addMenu("View");
    auto projection_menu = view_menu->addMenu("Projection");
    projection_menu->addAction(perspective_action);
    projection_menu->addAction(orthogonal_action);
    auto projections = new QActionGroup(projection_menu);
    for (auto p : {perspective_action, orthogonal_action})
    {
        projections->addAction(p);
        p->setCheckable(true);
    }
    projections->setExclusive(true);
    QObject::connect(projections, &QActionGroup::triggered,
                     this, &STLWindow::on_projection);

    resize(600, 400);
}

void STLWindow::on_open()
{
    QString filename = QFileDialog::getOpenFileName(this, "Load .stl file", QString(), "*.stl");
    if (!filename.isNull())
    {
        load_stl(filename);
    }
}

void STLWindow::on_saveas()
{
    QString sOutputFileName = QFileDialog::getSaveFileName(this, tr("Select output STL file"), QCoreApplication::applicationDirPath(), tr("STL (*.stl)"));
    if (!sOutputFileName.isEmpty())
    {
        QFileInfo fi(currentSTLFileName);
        if (fi.exists())
        {
            QFile::copy(currentSTLFileName, sOutputFileName);
        }
    }
}

void STLWindow::on_bad_stl()
{
    QMessageBox::critical(this, "Error",
                          "<b>Error:</b><br>"
                          "This <code>.stl</code> file is invalid or corrupted.<br>"
                          "Please export it from the original source, verify, and retry.");
}

void STLWindow::on_empty_mesh()
{
    QMessageBox::critical(this, "Error",
                          "<b>Error:</b><br>"
                          "This file is syntactically correct<br>but contains no triangles.");
}

void STLWindow::on_confusing_stl()
{
    QMessageBox::warning(this, "Warning",
                         "<b>Warning:</b><br>"
                         "This <code>.stl</code> file begins with <code>solid </code>but appears to be a binary file.<br>"
                         "<code>fstl</code> loaded it, but other programs may be confused by this file.");
}

void STLWindow::on_missing_file()
{
    QMessageBox::critical(this, "Error",
                          "<b>Error:</b><br>"
                          "The target file is missing.<br>");
}

void STLWindow::set_watched(const QString& filename)
{
    const auto files = watcher->files();
    if (files.size())
    {
        watcher->removePaths(watcher->files());
    }
    watcher->addPath(filename);
}

void STLWindow::on_projection(QAction* proj)
{
    if (proj == perspective_action)
    {
        canvas->view_perspective();
    }
    else
    {
        canvas->view_orthographic();
    }
}

void STLWindow::on_watched_change(const QString& filename)
{
    load_stl(filename, true);
}

void STLWindow::on_autoreload_triggered(bool b)
{
    if (b)
    {
        on_reload();
    }
}

void STLWindow::on_load_recent(QAction* a)
{
    load_stl(a->data().toString());
}

void STLWindow::on_reload()
{
    auto fs = watcher->files();
    if (fs.size() == 1)
    {
        load_stl(fs[0], true);
    }
}

bool STLWindow::load_stl(const QString& filename, bool is_reload)
{
    currentSTLFileName.clear();
    canvas->set_status("Loading " + filename);

    Loader* loader = new Loader(this, filename, is_reload);

    connect(loader, &Loader::got_mesh,
            canvas, &Canvas::load_mesh);
    connect(loader, &Loader::error_bad_stl,
              this, &STLWindow::on_bad_stl);
    connect(loader, &Loader::error_empty_mesh,
              this, &STLWindow::on_empty_mesh);
    connect(loader, &Loader::warning_confusing_stl,
              this, &STLWindow::on_confusing_stl);
    connect(loader, &Loader::error_missing_file,
              this, &STLWindow::on_missing_file);
    connect(loader, &Loader::finished,
            loader, &Loader::deleteLater);
    connect(loader, &Loader::finished,
            canvas, &Canvas::clear_status);

    if (filename[0] != ':')
    {
        connect(loader, &Loader::loaded_file,
                  this, &STLWindow::setWindowTitle);
        connect(loader, &Loader::loaded_file,
                  this, &STLWindow::set_watched);
    }

    loader->start();
    currentSTLFileName = filename;
    return true;
}

void STLWindow::viewOrthographic()
{
     canvas->view_orthographic();
     orthogonal_action->setChecked(true);
}

void STLWindow::viewPerspective()
{
     canvas->view_perspective();
     perspective_action->setChecked(true);
}

void STLWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        auto urls = event->mimeData()->urls();
        if (urls.size() == 1 && urls.front().path().endsWith(".stl"))
            event->acceptProposedAction();
    }
}

void STLWindow::dropEvent(QDropEvent *event)
{
    load_stl(event->mimeData()->urls().front().toLocalFile());
}
