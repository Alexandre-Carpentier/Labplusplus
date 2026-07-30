/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     27/07/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "instrument.h"
#include <vector>
#include <memory>
#include <string>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();

    // Save instruments configuration to Lua file
    bool SaveConfig(const std::string& filename, 
                    const std::vector<std::unique_ptr<IInstrument>>& instruments);

    // Load instruments configuration from Lua file
    std::vector<std::unique_ptr<IInstrument>> LoadConfig(const std::string& filename);

    // Get/Set config file path
    void SetConfigPath(const std::string& path) { configPath = path; }
    std::string GetConfigPath() const { return configPath; }

private:
    lua_State* L;
    std::string configPath;

    bool ExecuteLuaFile(const std::string& filename);
};