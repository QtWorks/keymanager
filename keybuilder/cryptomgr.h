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
    void writeInitialCryptedFile(const QString &sInputFilePath);

    //! Validate answer
    bool validateAnswer(const QString &sAnswer);

private:
    //! First installation?
    bool m_bFirstInstall;

    //! Encoder
    Encoder m_encoder;

    //! Disk serial hash
    QString m_sDiskSerialHash;

    //! Key 1
    QString m_sSecretKey1;

    //! Key 2
    QString m_sSecretKey2;

    //! Simple encoding order
    QVector<int> m_vSimpleEncodingOrder;

    //! Question
    QString m_sQuestion;
};

#endif // CRYPTOMGR_H
