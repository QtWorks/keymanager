// Qt
#include <QSettings>
#include <QCryptographicHash>

// Application
#include "cryptomgr.h"
#include "utils.h"
#include "helper.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

CryptoMgr::CryptoMgr(QObject *parent) : QObject(parent)
{
    m_sDiskSerial = Utils::getDiskSerial();
    m_sKey1 = "d8ffIUHGdf8g7b45";
    QSettings settings(":/ini/settings.ini", QSettings::IniFormat);
    m_bFirstInstall = settings.value(FIRST_INSTALL).toString() == VALUE_TRUE;
}

//-------------------------------------------------------------------------------------------------

CryptoMgr::~CryptoMgr()
{

}

//-------------------------------------------------------------------------------------------------

void CryptoMgr::writeInitialCryptedFile(const QString &sInputFilePath)
{
    QString sOriginalSCADScript = Utils::loadFile(sInputFilePath);
    if (!sOriginalSCADScript.simplified().isEmpty())
    {
        QByteArray baOriginalSCADScript = sOriginalSCADScript.toLatin1().toBase64();
        m_encoder.setKey(m_sKey1);
        QString sEncrypted = m_encoder.encrypt(QString::fromLatin1(baOriginalSCADScript));
        QString sOutputFileName = "c:/temp/script_in.enc";
        Utils::saveFile(sEncrypted, sOutputFileName);
    }
    else logError("ORIGINAL SCAD SCRIPT IS EMPTY");

    QString sEncrypted = Utils::loadFile("c:/temp/script_in.enc");
    if (m_bFirstInstall)
    {
        // Encode with random
        m_sRandom = Utils::randHex();
        m_encoder.setKey(m_sRandom);
        sEncrypted = m_encoder.encrypt(sEncrypted);

        // Encode width disk UID hash
        QString sDiskSerial = Utils::getDiskSerial();
        QString sDiskMD5Hash = QCryptographicHash::hash(sDiskSerial.toLatin1(), QCryptographicHash::Md5);
        m_encoder.setKey(sDiskMD5Hash);
        sEncrypted = m_encoder.encrypt(sEncrypted);
    }
}

//-------------------------------------------------------------------------------------------------

bool CryptoMgr::validateLicense(const QString &sQuestion, const QString &sAnswer)
{
    // TO DO
    return true;
}
