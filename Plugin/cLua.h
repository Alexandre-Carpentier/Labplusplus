#ifndef _LUA_H_
#define _LUA_H_
#include <iostream>
#include <format>
extern "C"
{
#include "cLua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#ifdef _WIN32
#pragma comment (lib, "lua54.lib")
#endif

#include "cDeviceBuilder.h"

cDevice* lua_start_vm(void* wxInst);
void lua_display_stack(lua_State* L);

static int lua_CreateBuilder(lua_State* L);
static int luaProduceProtocol(lua_State* L);
static int luaProducePanel(lua_State* L);
static int luaProduceImage(lua_State* L);
static int luaAddPanelCtrl(lua_State* L);
static int luaGetObject(lua_State* L);
static int lua_DestroyBuilder(lua_State* L);

static void registerObj(lua_State* L)
{
	/**************
	*
	* Builder
	*/
	lua_register(L, "builder", lua_CreateBuilder);
	luaL_newmetatable(L, "builder");
	lua_pushcfunction(L, lua_DestroyBuilder);
	lua_setfield(L, -2, "__gc");
	lua_pushvalue(L, -1); 
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, luaProduceProtocol); 
	lua_setfield(L, -2, "ProduceProtocol");
	lua_pushcfunction(L, luaProducePanel); 
	lua_setfield(L, -2, "ProducePanel");
	lua_pushcfunction(L, luaProduceImage); 
	lua_setfield(L, -2, "ProduceImage");
	lua_pushcfunction(L, luaAddPanelCtrl); 
	lua_setfield(L, -2, "AddPanelCtrl");
	lua_pushcfunction(L, luaGetObject);
	lua_setfield(L, -2, "GetObject");
	lua_pop(L, 1);
}


static int lua_CreateBuilder(lua_State* L)
{
	std::cout << "[*] lua_CreateObject\n";
	/*
	// Get object name from lua
	std::string obj_str = lua_tostring(L, 1);
	OBJTYPE type = OBJTYPE::OSCOPE;

	// Get object protocol from lua
	std::string protocol_str = lua_tostring(L, 1);
	SCPIMODE protocol = SCPIMODE::COM;

	// Build desired object
	if (obj_str.compare("oscope") == 0)
	{
		type = OBJTYPE::OSCOPE;

		if (protocol_str.compare("visa") == 0)
		{
			protocol = SCPIMODE::COM;
		}
	}
	*/

	*reinterpret_cast<DeviceBuilder1**>(lua_newuserdata(L, sizeof(DeviceBuilder1*))) = new DeviceBuilder1();
	// save object created 
	luaL_setmetatable(L, "builder");

	return 1;
}

static int luaProduceProtocol(lua_State* L) 
{
		int ret = lua_tointeger(L, 2);

		SCPIMODE protocol = SCPIMODE::COM; 

		switch (ret)
		{
		case SCPIMODE::COM:
			protocol = SCPIMODE::COM;
			break;

		case SCPIMODE::TCP:
			protocol = SCPIMODE::TCP;
			break;

		case SCPIMODE::USB:
			protocol = SCPIMODE::USB;
			break;

		case SCPIMODE::DAQMX:
			protocol = SCPIMODE::DAQMX;
			break;

		case SCPIMODE::VISA:
			protocol = SCPIMODE::VISA;
			break;

		}

	(*reinterpret_cast<DeviceBuilder1**>(luaL_checkudata(L, 1, "builder")))->ProduceProtocol(protocol);
	return 0;
}

static int luaProducePanel(lua_State* L) 
{
	lua_display_stack(L);
	wxWindow* wxInst = (wxWindow*) lua_touserdata(L, -1);
	(*reinterpret_cast<DeviceBuilder1**>(luaL_checkudata(L, 1, "builder")))->ProducePanel(wxInst);
	return 1;
}

static int luaProduceImage(lua_State* L) 
{
	lua_display_stack(L);
	wxWindow* wxInst = (wxWindow*)lua_touserdata(L, -1);
	(*reinterpret_cast<DeviceBuilder1**>(luaL_checkudata(L, 1, "builder")))->ProduceImage(wxInst);
	return 1;
}

static int luaAddPanelCtrl(lua_State* L) 
{
	lua_display_stack(L);
	CONTROLTYPE type = (CONTROLTYPE) lua_tointeger(L, 2);
	int length = lua_tointeger(L, 3);
	int height = lua_tointeger(L, 4);
	std::string name = lua_tostring(L, 5);

	(*reinterpret_cast<DeviceBuilder1**>(luaL_checkudata(L, 1, "builder")))->AddPanelCtrl(type, length, height, name);
	return 1;
}

static int luaGetObject(lua_State* L)
{
	lua_pushlightuserdata(L, (*reinterpret_cast<DeviceBuilder1**>(luaL_checkudata(L, 1, "builder")))->GetObject());
	return 1;
	 
}

// Destroy object with the lua garbage collector
static int lua_DestroyBuilder(lua_State* L)
{
	//delete* reinterpret_cast<DeviceBuilder1**>(lua_touserdata(L, 1));
	return 0;
}




#endif