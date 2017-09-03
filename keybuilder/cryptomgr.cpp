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

CryptoMgr::CryptoMgr(bool bFirstInstallation, QObject *parent) : QObject(parent),
    m_bFirstInstall(bFirstInstallation), m_sDiskSerialHash(""),
    m_sSecretKey1(""), m_sSecretKey2(""), m_sQuestion("")
{
    // Encoding order
    QString sOrder = "33, 8, 65, 73, 30, 72, 58, 16, 79, 1, 67, 14, 6, 4, 46, 44, 75, 31, 12, 78, 38, 42, 59, 35, 41, 63, 28, 15, 45, 68, 39, 32, 43, 5, 13, 76, 18, 49, 36, 56, 24, 69, 7, 53, 20, 48, 55, 64, 22, 11, 23, 57, 17, 29, 21, 25, 37, 77, 62, 70, 26, 74, 40, 34, 66, 61, 2, 50, 19, 10, 52, 54, 9, 27, 47, 0, 71, 51, 3, 60";
    QStringList lOrder = sOrder.split(",");
    for (int i=0; i<lOrder.length(); i++)
        m_vSimpleEncodingOrder << lOrder[i].toInt();

    // Compute disk serial hash
    m_sDiskSerialHash = Utils::getDiskSerialHash();

    // Key1 (known)
    m_sSecretKey1 = "d8ffIUHGdf8g7b45";

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
        m_encoder.setKey(m_sSecretKey1);
        QString sEncrypted = m_encoder.encrypt(QString::fromLatin1(baOriginalSCADScript));
        QString sOutputFileName = "c:/temp/script_in_before_install.enc";
        Utils::saveFile(sEncrypted, sOutputFileName);
    }
    else logError("ORIGINAL SCAD SCRIPT IS EMPTY");

    QString sEncrypted = Utils::loadFile("c:/temp/script_in.enc");
    if (m_bFirstInstall)
    {
        // Encode with random
        m_sSecretKey2 = Utils::randHex();
        qDebug() << "SECRET KEY2 = " << m_sSecretKey2;
        m_encoder.setKey(m_sSecretKey2);
        sEncrypted = m_encoder.encrypt(sEncrypted);

        // Encode with disk serial hash
        m_encoder.setKey(m_sDiskSerialHash);
        sEncrypted = m_encoder.encrypt(sEncrypted);

        // Build question
        m_sQuestion = Utils::simpleEncode(m_sSecretKey2 + m_sDiskSerialHash, m_vSimpleEncodingOrder);
    }
}

//-------------------------------------------------------------------------------------------------

bool CryptoMgr::validateAnswer(const QString &sAnswer)
{
    m_encoder.setKey(m_sSecretKey2);
    QString sValidAnswer = m_encoder.encrypt(m_sSecretKey1);
    if (sAnswer == sValidAnswer)
    {
        qDebug() << "*** VALID ANSWER ***";
        QString sCurrentDiskSerialHash = Utils::getDiskSerialHash();
        QString sExpectedQuestion = Utils::simpleEncode(m_sSecretKey2 + sCurrentDiskSerialHash, m_vSimpleEncodingOrder);
        if (sExpectedQuestion == m_sQuestion)
            return true;
    }
    qDebug() << "*** NOT VALID ANSWER ***";
    return false;
}

//-------------------------------------------------------------------------------------------------

const QString &CryptoMgr::question() const
{
    return m_sQuestion;
}
