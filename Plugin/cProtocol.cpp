#include "cProtocol.h"
//////////////////////////////////////////////////////////////
// COM CLASS
// -----------------------------------------------------------
// OPEN 
// SEND 
// RECV 
// CLOSE 
// 
// 
bool plug_cCom::open(std::string addr)
{
    return true;
}
bool plug_cCom::send(std::string cmd)
{
    return true;
}
bool plug_cCom::recv(std::string& response)
{
    return true;
}
void plug_cCom::close()
{
    return;
}
//////////////////////////////////////////////////////////////
// TCP CLASS
// -----------------------------------------------------------
// OPEN 
// SEND 
// RECV 
// CLOSE 
// 
// 
bool plug_cTcp::open(std::string addr)
{
    return true;
}
bool plug_cTcp::send(std::string cmd)
{
    return true;
}
bool plug_cTcp::recv(std::string& response)
{
    return true;
}
void plug_cTcp::close()
{
    return;
}
//////////////////////////////////////////////////////////////
// USB CLASS
// -----------------------------------------------------------
// OPEN 
// SEND 
// RECV 
// CLOSE 
// 
// 
bool plug_cUsb::open(std::string addr)
{
    return true;
}
bool plug_cUsb::send(std::string cmd)
{
    return true;
}
bool plug_cUsb::recv(std::string& response)
{
    return true;
}
void plug_cUsb::close()
{
    return;
}
//////////////////////////////////////////////////////////////
// DAQMX CLASS
// -----------------------------------------------------------
// OPEN 
// SEND 
// RECV 
// CLOSE 
// 
// 
bool plug_cDaq::open(std::string addr)
{
    return true;
}
bool plug_cDaq::send(std::string cmd)
{
    return true;
}
bool plug_cDaq::recv(std::string& response)
{
    return true;
}
void plug_cDaq::close()
{
    return;
}
//////////////////////////////////////////////////////////////
// VISA CLASS
// -----------------------------------------------------------
// OPEN 
// SEND 
// RECV 
// CLOSE 
// 
// 
bool plug_cVisa::open(std::string addr)
{
    return true;
}
bool plug_cVisa::send(std::string cmd)
{
    return true;
}
bool plug_cVisa::recv(std::string& response)
{
    return true;
}
void plug_cVisa::close()
{
    return;
}
