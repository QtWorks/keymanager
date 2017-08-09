// Qt
#include <QApplication>
#include <QDebug>

// Application
#include "keybuilder.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KeyBuilder *pKeyBuilder = new KeyBuilder();
    pKeyBuilder->startup();
    int res = app.exec();
    pKeyBuilder->shutdown();
    delete pKeyBuilder;

    return res;
}
