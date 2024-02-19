#include "cLua.h"

bool CheckLua(lua_State* L, int res)
{
	if (res != LUA_OK)
	{
		std::string err = lua_tostring(L, -1);
		std::cout << err << std::endl;
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////
// LUA GET SCRIPT PATH
// -----------------------------------------------------------
// 
// 
// 
// 
//
// 
std::string lua_get_script_path()
{
	wchar_t szFileName[MAX_PATH];
	memset(szFileName, 0, sizeof(szFileName));
	GetModuleFileName((HINSTANCE)&__ImageBase, szFileName, MAX_PATH);

	size_t len = wcslen(szFileName);
	size_t begin = 0;
	size_t filename_length = 0;
	for (int end = len; end > 0; end--)
	{
		filename_length++;
		if (szFileName[end] == '\\')
		{
			// Save index
			begin = end + 1;
			break;
		}
	}

	//wprintf(L"Filename: %s\n", szFileName);

		// Calc difference
	size_t diff = len - begin;
	if (diff > 0) // there is \\ in the path
	{
		wcsncpy(szFileName, (szFileName)+begin, diff);
	}
	//wprintf(L"Filename: %s\n", szFileName);
	szFileName[diff] = '\0';
	len = wcslen(szFileName);
	szFileName[len - 4] = '\0'; // Cut filepath without .dll (4*wchar_t)
	//wprintf(L"Filename: %s\n", szFileName);

	std::wstring lua_file = L"ADDON\\";
	lua_file.append(szFileName);
	lua_file.append(L".lua");

	int count = WideCharToMultiByte(CP_UTF8, 0, lua_file.c_str(), lua_file.length(), NULL, 0, NULL, NULL);
	std::string str(count, 0);
	WideCharToMultiByte(CP_UTF8, 0, lua_file.c_str(), -1, &str[0], count, NULL, NULL);

	return str;
}

void lua_display_stack(lua_State* L)
{
	int top = lua_gettop(L);

	std::string str = "From top to bottom, the lua stack is \n";
	for (unsigned index = top; index > 0; index--)
	{
		int type = lua_type(L, index);
		std::string index_str = std::format("{} ", index);
		switch (type)
		{
			

			// booleans
		case LUA_TBOOLEAN:
			str = index_str + str + (lua_toboolean(L, index) ? "true" : "false") + "\n";
			break;

			// numbers
		case LUA_TNUMBER:
			str = index_str + str + std::to_string(lua_tonumber(L, index)) + "\n";
			break;

			// strings
		case LUA_TSTRING:
			str = index_str + str + lua_tostring(L, index) + "\n";
			break;

			// other
		default:
			str = str + index_str +lua_typename(L, type) + "\n";
			break;
		}
	}

	str = str + "\n";
	std::cout << str;
}
//////////////////////////////////////////////////////////////
// LUA VM
// -----------------------------------------------------------
// 
// 
// 
// 
//
// 

cDevice* lua_start_vm(void* wxInst)
{
	DeviceBuilder1* builder = nullptr;
	cDevice* dev = nullptr;

	std::cout << "[LUA] Starting Lua VM...\n";

		// Start a new private state by plugin

	lua_State* L = nullptr;
	L = luaL_newstate();
	luaL_openlibs(L);

		// Register function and class between c++ to lua

	registerObj(L);

		// Get the lua script path for the current plugin

	std::cout << "[LUA] Loading script...\n";
	std::string script_path = lua_get_script_path();
	
		// Run the script

	if (CheckLua(L, luaL_dofile(L, script_path.c_str())))
	{

			// Calling entrypoint
			// build interface here

		std::cout << "[LUA] Running lua_entrypoint...\n";
		lua_getglobal(L, "lua_entrypoint");
		if (lua_isfunction(L, -1))
		{			
				// Call entrypoint with wxInstance arg
			lua_pushlightuserdata(L, wxInst); // push pointer to lua
			lua_pcall(L, /*1 arg*/1, /*1 return*/1, 0);

			if (lua_isuserdata(L, -1))
			{
				// TODO: mauvais index builder corompu
				lua_display_stack(L);

				builder = (DeviceBuilder1*) lua_touserdata(L, -1);
				dev = builder->GetProduct();
				dev->set_lua_state_ptr(L);

				// Size properly items inside the sizer
				dev->header_v_sizer->Add(dev->header_h_sizer); // Header
				dev->header_v_sizer->Add(dev->grid_sizer); // Grid
				dev->panel->SetSizerAndFit(dev->header_v_sizer); // Main vertical sizer
				dev->DisplayConfiguration();
			}
		}
	
			// Calling init
			// 

		std::cout << "[LUA] Running lua_init...\n";
		lua_getglobal(L, "lua_init");
		if (lua_isfunction(L, -1))
		{
			lua_pushstring(L, "/Dev1");
			CheckLua(L, lua_pcall(L, 1, 1, 0));
		}
	}

	return dev;
}


bool lua_stop_vm(cDevice* dev)
{
	lua_State* L = static_cast<lua_State *> (dev->get_lua_state_ptr());

	lua_getglobal(L, "lua_close");
	if (lua_isfunction(L, -1))
	{
		CheckLua(L, lua_pcall(L, 0, 0, 0));
		//lua_close(L); // do not forget to free memory
		return true;
	}
	return false;
}