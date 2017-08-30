#ifndef ENCODER_H_INCLUDED
#define ENCODER_H_INCLUDED

// Qt
#include <QString>

class Encoder
{
public:
    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructor
    Encoder(const QString &sKeyIn);

    //! Destructor
    Encoder();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //! Encrypt
    QString encrypt(const QString &sPlainText);

    //! Decrypt
    QString decrypt(const QString &sPlainText);

    //! Set key
    void setKey(const QString &sKeyIn);

    //! Get key
    const std::vector<int>& getKey() const;

    //! Return key size
    size_t keySize() const;

private:
    //! Init
    void init(const QString &sKeyIn);

    //! Own data
    std::map<char,int> invAlphabet;
    std::string alphabet;
    std::vector<int> key;
    int ctr;
    bool encflg;
};

#endif // ENCODER_H_INCLUDED
