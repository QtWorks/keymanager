#ifndef HELPER_H
#define HELPER_H

// Qt
#include <QString>

// Application
#define INFO_ON true
#define ERROR_ON true
#define WARNING_ON true

//! Log information
void logInfo(const QString &sMsg);

//! Log error
void logError(const QString &sMsg);

//! Log warning
void logWarning(const QString &sMsg);

#endif // HELPER_H
