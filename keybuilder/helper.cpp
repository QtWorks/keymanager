// Qt
#include <QDebug>

// Application
#include "helper.h"

//-------------------------------------------------------------------------------------------------

void logInfo(const QString &sMsg)
{
    if (INFO_ON)
    {
        qDebug() << "INFORMATION: " << sMsg;
    }
}

//-------------------------------------------------------------------------------------------------

void logError(const QString &sMsg)
{
    if (ERROR_ON)
    {
        qDebug() << "ERROR: " << sMsg;
    }
}

//-------------------------------------------------------------------------------------------------

void logWarning(const QString &sMsg)
{
    if (WARNING_ON)
    {
        qDebug() << "WARNING: " << sMsg;
    }
}
