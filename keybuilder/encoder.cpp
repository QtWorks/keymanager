#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <iterator>
#include "encoder.h"

//-------------------------------------------------------------------------------------------------

Encoder::Encoder(const QString &sKeyIn)
{
    init(sKeyIn);
}

//-------------------------------------------------------------------------------------------------

Encoder::Encoder()
{
    init("default");
}

//-------------------------------------------------------------------------------------------------

QString Encoder::encrypt(const QString &sPlainText)
{
    if (key.size() > 0)
    {
        std::string plaintext = sPlainText.toStdString();
        std::string out;
        for (int i=0;i<plaintext.size();++i)
        {
            if (invAlphabet.count(plaintext[i]) > 0)
            {
                int k=invAlphabet[plaintext[i]];
                int j=(k>(alphabet.size()-1))?alphabet.size():0;
                out.push_back(alphabet[(k+key[ctr])%alphabet.size() + j]);
            }
            else
            {
                out.push_back(plaintext[i]);
                --ctr;
            }
            ctr=(ctr+1)%key.size();
        }
        return QString::fromStdString(out);
    }
    return QString("");
}

//-------------------------------------------------------------------------------------------------

QString Encoder::decrypt(const QString &sPlainText)
{
    std::string plaintext = sPlainText.toStdString();
    std::string out;
    for (int i=0;i<plaintext.size();++i)
    {
        if(invAlphabet.count(plaintext[i]) > 0)
        {
            int k=invAlphabet[plaintext[i]];
            int j=(k>(alphabet.size()-1))?alphabet.size():0;
            if(k-key[ctr]<0){k+=alphabet.size();};
            out.push_back(alphabet[(k-key[ctr])%alphabet.size() + j]);
        }
        else
        {
            out.push_back(plaintext[i]);
            --ctr;
        }
        ctr=(ctr+1)%key.size();
    }
    return QString::fromStdString(out);
}

//-------------------------------------------------------------------------------------------------

void Encoder::setKey(const QString &sKeyIn)
{
    std::string keyin = sKeyIn.toStdString();
    key.clear();
    for (int i=0;i<keyin.size();++i)
    {
        if (invAlphabet.count(keyin[i])>0)
            key.push_back(invAlphabet[keyin[i]]%alphabet.size());
    }
    ctr=0;
}

//-------------------------------------------------------------------------------------------------

const std::vector<int>& Encoder::getKey() const
{
    return key;
}

//-------------------------------------------------------------------------------------------------

size_t Encoder::keySize() const
{
    return key.size();
}

//-------------------------------------------------------------------------------------------------

void Encoder::init(const QString &sKeyIn)
{
    std::string keyin = sKeyIn.toStdString();
    alphabet ="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/=";
    for (int i=0;i<alphabet.size();++i)
    {
        invAlphabet[alphabet[i]]=i;
    }
    encflg=true;
    setKey(QString::fromStdString(keyin));
}



