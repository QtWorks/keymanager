// Qt
#include <QApplication>
#include <QDebug>
#include <QMessageBox>

// Application
#include "keybuilder.h"
#include "utils.h"
#include "encoder.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Return an instance of the application
    KeyBuilder *pKeyBuilder = KeyBuilder::instance();

    // Startup
    pKeyBuilder->startup(argv[1]);

    // Event loop
    int res = app.exec();

    // Shutdown
    pKeyBuilder->shutdown();

    // Release
    delete pKeyBuilder;

    return res;
}
