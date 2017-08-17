// Windows
#include <stdio.h>
#include <windows.h>

// Application
#include "utils.h"

//-------------------------------------------------------------------------------------------------

QString Utils::getDiskSerial()
{
    DWORD dwVolSerial;
    BOOL bIsRetrieved = GetVolumeInformation(L"C:\\", NULL, NULL, &dwVolSerial, NULL, NULL, NULL, NULL);
    if (bIsRetrieved)
        return QString::number(dwVolSerial);
    return QString("");
}
