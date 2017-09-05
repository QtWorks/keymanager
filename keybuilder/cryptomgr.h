#ifndef CRYPTOMGR_H
#define CRYPTOMGR_H

// Qt
#include <QObject>
#include <QVector>

// Application
#include "encoder.h"

class CryptoMgr : public QObject
{
    Q_OBJECT

public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    explicit CryptoMgr(bool bFirstInstallation, QObject *parent=nullptr);

    //! Destructor
    ~CryptoMgr();

    //-------------------------------------------------------------------------------------------------
    // Getters & setters
    //-------------------------------------------------------------------------------------------------

    //! Return question
    const QString &question() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Write initial crypted file
    void doInitialEncryption();

    //! Set answer
    void setAnswerPlusDiskSerialHash(const QString &sAnswer);

    //! Decrypt
    bool decrypt(QString &sClearScriptFile);

private:
    //! First installation?
    bool m_bFirstInstall;

    //! Encoder
    Encoder m_encoder;

    //! Disk serial hash
    QString m_sDiskSerialHash;

    //! Key 2
    QString m_sSecretKey2;

    //! Simple encoding order
    QVector<int> m_vSimpleEncodingOrder;

    //! Question
    QString m_sQuestion;

    //! Answer
    QString m_sAnswer;
};

#endif // CRYPTOMGR_H
