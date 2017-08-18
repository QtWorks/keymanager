#ifndef HELPER_H
#define HELPER_H

// Qt
#include <QString>

// Application
#define INFO_ON true
#define ERROR_ON false
#define WARNING_ON false

//! Log information
void logInfo(const QString &sMsg);

//! Log error
void logError(const QString &sMsg);

//! Log warning
void logWarning(const QString &sMsg);

#endif // HELPER_H
