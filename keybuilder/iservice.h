#ifndef ISERVICE_H
#define ISERVICE_H

class IService
{
public:
    virtual bool startup(const QString &args="") = 0;
    virtual void shutdown() = 0;
    virtual ~IService() {

    }
};

#endif // ISERVICE_H
