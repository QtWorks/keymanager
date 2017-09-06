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
    explicit CryptoMgr(QObject *parent=nullptr);

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
    void setAnswer(const QString &sAnswer);

    //! Set target serial hash
    void setTargetSerialHash(const QString &sTargetSerialHash);

    //! Decrypt
    bool decrypt(QString &sClearScriptFile);

private:
    //! Encoder
    Encoder m_encoder;

    //! Key 2
    QString m_sSecretKey2;

    //! Simple encoding order
    QVector<int> m_vSimpleEncodingOrder;

    //! Question
    QString m_sQuestion;

    //! Answer
    QString m_sAnswer;

    //! Target serial hash
    QString m_sTargetSerialHash;

signals:
    //! License error
    void licenseError();
};

#endif // CRYPTOMGR_H
