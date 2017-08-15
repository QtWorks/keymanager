#ifndef HELPER_H
#define HELPER_H

// Qt
#include <QDebug>
#define DEBUG_ON true

static void logMessage(const QString &sMsg)
{
    if (DEBUG_ON)
    {
        qDebug() << sMsg;
    }
}

#endif // HELPER_H
