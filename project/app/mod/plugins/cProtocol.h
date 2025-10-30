/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
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
class plug_cProtocol {
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
class plug_cCom : public plug_cProtocol
{
    virtual std::string get_type()override { return std::string("COM port"); };
    virtual bool open(std::string addr) override;
    virtual bool send(std::string cmd) override;
    virtual bool recv(std::string& response) override;
    virtual void close() override;
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
class plug_cTcp : public plug_cProtocol
{
    virtual std::string get_type()override { return std::string("TCP port"); };
    virtual bool open(std::string addr) override;
    virtual bool send(std::string cmd) override;
    virtual bool recv(std::string& response) override;
    virtual void close() override;
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
class plug_cUsb : public plug_cProtocol
{
    virtual std::string get_type()override { return std::string("USB port"); };
    virtual bool open(std::string addr) override;
    virtual bool send(std::string cmd) override;
    virtual bool recv(std::string& response) override;
    virtual void close() override;
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
class plug_cDaq : public plug_cProtocol
{
    virtual std::string get_type()override { return std::string("DAQMX"); };
    virtual bool open(std::string addr) override;
    virtual bool send(std::string cmd) override;
    virtual bool recv(std::string& response) override;
    virtual void close() override;
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
class plug_cVisa : public plug_cProtocol
{
    virtual std::string get_type()override { return std::string("VISA"); };
    virtual bool open(std::string addr) override;
    virtual bool send(std::string cmd) override;
    virtual bool recv(std::string& response) override;
    virtual void close() override;
};
#endif
