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
bool cCom::open(std::string addr)
{
    return true;
}
bool cCom::send(std::string cmd)
{
    return true;
}
bool cCom::recv(std::string& response)
{
    return true;
}
void cCom::close()
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
bool cTcp::open(std::string addr)
{
    return true;
}
bool cTcp::send(std::string cmd)
{
    return true;
}
bool cTcp::recv(std::string& response)
{
    return true;
}
void cTcp::close()
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
bool cUsb::open(std::string addr)
{
    return true;
}
bool cUsb::send(std::string cmd)
{
    return true;
}
bool cUsb::recv(std::string& response)
{
    return true;
}
void cUsb::close()
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
bool cDaq::open(std::string addr)
{
    return true;
}
bool cDaq::send(std::string cmd)
{
    return true;
}
bool cDaq::recv(std::string& response)
{
    return true;
}
void cDaq::close()
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
bool cVisa::open(std::string addr)
{
    return true;
}
bool cVisa::send(std::string cmd)
{
    return true;
}
bool cVisa::recv(std::string& response)
{
    return true;
}
void cVisa::close()
{
    return;
}
