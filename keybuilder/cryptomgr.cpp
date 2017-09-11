// Qt
#include <QSettings>
#include <QCryptographicHash>
#include <QDebug>
#include <QMessageBox>

// Application
#include "cryptomgr.h"
#include "utils.h"
#include "helper.h"
#include "constants.h"

//-------------------------------------------------------------------------------------------------

CryptoMgr::CryptoMgr(QObject *parent) : QObject(parent),
    m_sSecretKey2(""),  m_sQuestion(""),
    m_sAnswer(""), m_sTargetSerialHash("")
{
    // Encoding order
    QString sOrder = "33, 8, 65, 73, 30, 72, 58, 16, 79, 1, 67, 14, 6, 4, 46, 44, 75, 31, 12, 78, 38, 42, 59, 35, 41, 63, 28, 15, 45, 68, 39, 32, 43, 5, 13, 76, 18, 49, 36, 56, 24, 69, 7, 53, 20, 48, 55, 64, 22, 11, 23, 57, 17, 29, 21, 25, 37, 77, 62, 70, 26, 74, 40, 34, 66, 61, 2, 50, 19, 10, 52, 54, 9, 27, 47, 0, 71, 51, 3, 60";
    QStringList lOrder = sOrder.split(",");
    for (int i=0; i<lOrder.length(); i++)
        m_vSimpleEncodingOrder << lOrder[i].toInt();

    // Compute disk serial hash
    m_sTargetSerialHash = Utils::getDiskSerialHash();
    QString sMsg = QString("DISK SERIAL HASH IS: %1").arg(m_sTargetSerialHash);
    logInfo(sMsg);
}

//-------------------------------------------------------------------------------------------------

CryptoMgr::~CryptoMgr()
{

}

//-------------------------------------------------------------------------------------------------

void CryptoMgr::doInitialEncryption()
{
    QString sOutputFileName = Utils::outputDir().absoluteFilePath(ENCODED_SCRIPT_NAME);
    QString sEncrypted("");

    if (Utils::loadFile(sOutputFileName, sEncrypted))
    {
        // Encode with random
        m_sSecretKey2 = Utils::randHex();
        m_encoder.setKey(m_sSecretKey2);
        sEncrypted = m_encoder.encrypt(sEncrypted);

        // Encode with disk serial hash
        m_encoder.setKey(Utils::getDiskSerialHash());
        sEncrypted = m_encoder.encrypt(sEncrypted);
        Utils::saveFile(sEncrypted, sOutputFileName);

        // Build question
        m_sQuestion = Utils::simpleEncode(m_sSecretKey2 + Utils::getDiskSerialHash(), m_vSimpleEncodingOrder);
    }
    else
    {
        QString sMsg = QString("COULD NOT LOAD: %1").arg(sOutputFileName);
        logError(sMsg);
    }
}

//-------------------------------------------------------------------------------------------------

void CryptoMgr::setAnswer(const QString &sAnswer)
{
    m_sAnswer = sAnswer.mid(0, 16);
}

//-------------------------------------------------------------------------------------------------

void CryptoMgr::setTargetSerialHash(const QString &sTargetSerialHash)
{
    m_sTargetSerialHash = sTargetSerialHash;
}

//-------------------------------------------------------------------------------------------------

bool CryptoMgr::decrypt(QString &sClearScriptFile)
{
    sClearScriptFile.clear();
    QString sEncodedFileName = Utils::outputDir().absoluteFilePath(ENCODED_SCRIPT_NAME);
    QString sFileContents("");

    QString sMsg = QString("COMPARING SERIAL HASH: %1 and %2").arg(m_sTargetSerialHash).arg(Utils::getDiskSerialHash());
    logInfo(sMsg);

    if (m_sTargetSerialHash == Utils::getDiskSerialHash())
    {
        if (Utils::loadFile(sEncodedFileName, sFileContents))
        {
            QString sMsg = QString("DECRYPTING USING SERIAL HASH: %1").arg(Utils::getDiskSerialHash());
            logInfo(sMsg);

            // Decrypt using disk serial hash
            m_encoder.setKey(Utils::getDiskSerialHash());
            QString sDecrypted1 = m_encoder.decrypt(sFileContents);

            sMsg = QString("DECRYPTING USING ANSWER: %1").arg(m_sAnswer);
            logInfo(sMsg);

            // Decrypt using answer
            m_encoder.setKey(m_sAnswer);
            QString sDecrypted2 = m_encoder.decrypt(sDecrypted1);

            sMsg = QString("CHANGING FROM BASE64").arg(m_sAnswer);
            logInfo(sMsg);

            // Change from base64 to QString
            QByteArray baResult = QByteArray::fromBase64(sDecrypted2.toLatin1());
            QString sOut = QString::fromLatin1(baResult);

            // Write file
            sClearScriptFile = Utils::outputDir().absoluteFilePath(CLEAR_SCRIPT_FILE);
            sMsg = QString("WRITING CLEAR SCRIPT FILE: %1").arg(sClearScriptFile);
            logInfo(sMsg);
            return Utils::saveFile(sOut, sClearScriptFile);
        }
        else
        {
            QString sMsg = QString("COULD NOT LOAD: %1").arg(sEncodedFileName);
            logError(sMsg);
        }
    }
    else
    {
        emit licenseError();
        logError("THIS SOFTWARE IS NOT LICENSED FOR THAT MACHINE");
    }

    return false;
}

//-------------------------------------------------------------------------------------------------

const QString &CryptoMgr::question() const
{
    return m_sQuestion;
}


