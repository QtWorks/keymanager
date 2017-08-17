// Qt
#include <QApplication>
#include <QDebug>

// Application
#include "keybuilder.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    qDebug() << Utils::getDiskSerial();

    QApplication app(argc, argv);

    // Return an instance of the application
    KeyBuilder *pKeyBuilder = new KeyBuilder();

    // Startup
    pKeyBuilder->startup();

    // Event loop
    int res = app.exec();

    // Shutdown
    pKeyBuilder->shutdown();

    // Release
    delete pKeyBuilder;

    return res;
}
