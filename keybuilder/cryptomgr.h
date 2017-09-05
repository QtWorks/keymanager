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

    //! Return disk serial hash
    const QString &diskSerialHash() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Write initial crypted file
    void doInitialEncryption();

    //! Set answer
    void setAnswer(const QString &sAnswer);

    //! Decrypt
    bool decrypt(QString &sClearScriptFile);

private:
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

    //! Target serial hash
    QString m_sTargetSerialHash;
};

#endif // CRYPTOMGR_H
