#ifndef CRYPTOMGR_H
#define CRYPTOMGR_H

// Qt
#include <QObject>

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

    //! Write initial crypted file
    void writeInitialCryptedFile(const QString &sInputFilePath);

    //! Validate license
    bool validateLicense(const QString &sQuestion, const QString &sAnswer);

private:
    //! First installation?
    bool m_bFirstInstall;

    //! Encoder
    Encoder m_encoder;

    //! Disk serial
    QString m_sDiskSerial;

    //! Key 1
    QString m_sKey1;

    //! Random
    QString m_sRandom;

signals:

public slots:
};

#endif // CRYPTOMGR_H
