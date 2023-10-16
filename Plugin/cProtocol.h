#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include <string>
//////////////////////////////////////////////////////////////
// BASE CLASS
// -----------------------------------------------------------
// OPEN = 0
// SEND = 0
// RECV = 0
// CLOSE = 0
// 
// 
class cProtocol {
public:
    virtual std::string get_type() = 0;
    virtual bool open(std::string addr) = 0;
    virtual bool send(std::string cmd) = 0;
    virtual bool recv(std::string& response) = 0;
    virtual void close() = 0;
};
//////////////////////////////////////////////////////////////
// COM CLASS
// -----------------------------------------------------------
// OPEN 
// SEND 
// RECV 
// CLOSE 
// 
// 
class cCom : public cProtocol
{
    std::string get_type()override { return std::string("COM port"); };
    bool open(std::string addr) override;
    bool send(std::string cmd) override;
    bool recv(std::string& response) override;
    void close() override;
};
//////////////////////////////////////////////////////////////
// TCP CLASS
// -----------------------------------------------------------
// OPEN 
// SEND 
// RECV 
// CLOSE 
// 
// 
class cTcp : public cProtocol
{
    std::string get_type()override { return std::string("TCP port"); };
    bool open(std::string addr) override;
    bool send(std::string cmd) override;
    bool recv(std::string& response) override;
    void close() override;
};
//////////////////////////////////////////////////////////////
// USB CLASS
// -----------------------------------------------------------
// OPEN 
// SEND 
// RECV 
// CLOSE 
// 
// 
class cUsb : public cProtocol
{
    std::string get_type()override { return std::string("USB port"); };
    bool open(std::string addr) override;
    bool send(std::string cmd) override;
    bool recv(std::string& response) override;
    void close() override;
};
//////////////////////////////////////////////////////////////
// DAQMX CLASS
// -----------------------------------------------------------
// OPEN 
// SEND 
// RECV 
// CLOSE 
// 
// 
class cDaq : public cProtocol
{
    std::string get_type()override { return std::string("DAQMX linked"); };
    bool open(std::string addr) override;
    bool send(std::string cmd) override;
    bool recv(std::string& response) override;
    void close() override;
};
//////////////////////////////////////////////////////////////
// VISA CLASS
// -----------------------------------------------------------
// OPEN 
// SEND 
// RECV 
// CLOSE 
// 
// 
class cVisa : public cProtocol
{
    std::string get_type()override { return std::string("VISA linked"); };
    bool open(std::string addr) override;
    bool send(std::string cmd) override;
    bool recv(std::string& response) override;
    void close() override;
};
#endif