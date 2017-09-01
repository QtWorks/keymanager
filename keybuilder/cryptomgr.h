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

    //! Return response
    const QString &answer() const;

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Write initial crypted file
    void writeInitialCryptedFile(const QString &sInputFilePath);

    //! Validate license
    bool validateLicense(const QString &sQuestion, const QString &sAnswer);

private:
    //! First installation?
    bool m_bFirstInstall;

    //! Encoder
    Encoder m_encoder;

    //! Disk serial hash
    QString m_sDiskSerialHash;

    //! Key 1
    QString m_sKey1;

    //! Key 2
    QString m_sKey2;

    //! Simple encoding order
    QVector<int> m_vSimpleEncodingOrder;

    //! Question
    QString m_sQuestion;

    //! Reponse
    QString m_sResponse;
};

#endif // CRYPTOMGR_H
