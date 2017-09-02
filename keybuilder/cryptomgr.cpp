// Qt
#include <QSettings>
#include <QCryptographicHash>
#include <QDebug>

// Application
#include "cryptomgr.h"
#include "utils.h"
#include "helper.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

CryptoMgr::CryptoMgr(QObject *parent) : QObject(parent),
    m_bFirstInstall(true), m_sDiskSerialHash(""),
    m_sKey1(""), m_sKey2(""), m_sQuestion(""), m_sResponse("")
{
    // Compute disk serial hash
    m_sDiskSerialHash = Utils::getDiskSerialHash();

    // Key1 (known)
    m_sKey1 = "d8ffIUHGdf8g7b45";

    // Check if this is the first installation
    QSettings settings(":/ini/settings.ini", QSettings::IniFormat);
    m_bFirstInstall = settings.value(FIRST_INSTALL).toString() == VALUE_TRUE;

    // Compute simple encoding order (known)
    m_vSimpleEncodingOrder = Utils::buildOrder(QUESTION_LENGTH);

    writeInitialCryptedFile(":/data/script_in.scad");
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
        QString sOutputFileName = "c:/temp/script_in_before_install.enc";
        Utils::saveFile(sEncrypted, sOutputFileName);
    }
    else logError("ORIGINAL SCAD SCRIPT IS EMPTY");

    QString sEncrypted = Utils::loadFile("c:/temp/script_in.enc");
    if (m_bFirstInstall)
    {
        // Encode with random
        m_sKey2 = Utils::randHex();
        m_encoder.setKey(m_sKey2);
        sEncrypted = m_encoder.encrypt(sEncrypted);

        // Encode with disk serial hash
        m_encoder.setKey(m_sDiskSerialHash);
        sEncrypted = m_encoder.encrypt(sEncrypted);

        // Build question
        m_sQuestion = Utils::simpleEncode(m_sKey2 + m_sDiskSerialHash, m_vSimpleEncodingOrder);

        // Compute response
        m_encoder.setKey(m_sKey2);
        m_sResponse = m_encoder.encrypt(m_sKey1).toLatin1().toHex().toUpper();

        // Output file name
        QString sOutputFileName = "c:/temp/script_in_after_install.enc";
        Utils::saveFile(sEncrypted, sOutputFileName);
    }
}

//-------------------------------------------------------------------------------------------------

bool CryptoMgr::validateLicense(const QString &sQuestion, const QString &sAnswer)
{
    // TO DO
    return true;
}

//-------------------------------------------------------------------------------------------------

const QString &CryptoMgr::question() const
{
    return m_sQuestion;
}

//-------------------------------------------------------------------------------------------------

const QString &CryptoMgr::answer() const
{
    return m_sResponse;
}
