#ifndef _LUA_H_
#define _LUA_H_
extern "C"
{
#include "cLua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#ifdef _WIN32
#pragma comment (lib, "lua54.lib")
#endif
#endif

// Here define all the binded fonction you can call from LUA

// Device Setter
int lua_SetDeviceName(lua_State* L);
int lua_SetDeviceAddr(lua_State* L);
int lua_SetDeviceProtocol(lua_State* L);
int lua_SetDeviceUnit(lua_State* L);
int lua_SetDeviceIdentificationStr(lua_State* L);
int lua_SetDeviceImg(lua_State* L);

// Plugin builder
int lua_SetControl(lua_State* L);
//builder->AddPanelCtrl(CONTROLTYPE::BUT, 150, 20, "Query test equipment?");



int lua_CreateObject(lua_State* L);

