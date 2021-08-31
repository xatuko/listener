#ifndef ITCPSERVER_H
#define ITCPSERVER_H

#include "common.h"

class ITcpServer
{
public:
    ITcpServer() = default;
    virtual ~ITcpServer() = default;

    virtual void setAddr(const std::string & ip, const int & port) = 0;
    virtual int init() = 0;
    virtual int startListen() = 0;
    virtual bool getMessage(ByteArray & message) = 0;
    virtual void closeSock() = 0;
};

#endif // ITCPSERVER_H