/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     27/07/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "configManager.h"
#include "instrumentFactory.h"
#include <fstream>
#include <iostream>

ConfigManager::ConfigManager() : configPath("config.lua") {
    L = luaL_newstate();
    luaL_openlibs(L);
}

ConfigManager::~ConfigManager() {
    if (L) {
        lua_close(L);
    }
}

bool ConfigManager::SaveConfig(const std::string& filename, 
                                const std::vector<std::unique_ptr<IInstrument>>& instruments) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file for writing: " << filename << std::endl;
        return false;
    }

    file << "-- Lab++ Instrument Configuration\n";
    file << "-- Auto-generated configuration file\n\n";
    file << "instruments = {\n";

    for (size_t i = 0; i < instruments.size(); ++i) {
        file << "  {\n";
        file << "    type = \"" << instruments[i]->GetType() << "\",\n";
        file << "    name = \"" << instruments[i]->GetName() << "\",\n";
        file << "    config = [[\n";
        file << instruments[i]->SerializeToLua();
        file << "    ]]\n";
        file << "  }";
        if (i < instruments.size() - 1) {
            file << ",";
        }
        file << "\n";
    }

    file << "}\n\n";
    file << "return instruments\n";

    file.close();
    std::cout << "Configuration saved to: " << filename << std::endl;
    return true;
}

std::vector<std::unique_ptr<IInstrument>> ConfigManager::LoadConfig(const std::string& filename) {
    std::vector<std::unique_ptr<IInstrument>> instruments;

    if (!ExecuteLuaFile(filename)) {
        std::cerr << "Failed to load config file: " << filename << std::endl;
        return instruments;
    }

    // Get the instruments table
    lua_getglobal(L, "instruments");
    if (!lua_istable(L, -1)) {
        std::cerr << "Config file doesn't contain 'instruments' table" << std::endl;
        lua_pop(L, 1);
        return instruments;
    }

    // Iterate through instruments array
    size_t numInstruments = lua_rawlen(L, -1);
    for (size_t i = 1; i <= numInstruments; ++i) {
        lua_rawgeti(L, -1, i);
        
        if (lua_istable(L, -1)) {
            // Get type
            lua_getfield(L, -1, "type");
            const char* type = lua_tostring(L, -1);
            lua_pop(L, 1);

            // Get name
            lua_getfield(L, -1, "name");
            const char* name = lua_tostring(L, -1);
            lua_pop(L, 1);
            
            // Get config
            lua_getfield(L, -1, "config");
            const char* config = lua_tostring(L, -1);
            lua_pop(L, 1);

            if (type) {
                // Create instrument using factory
                auto instrument = InstrumentFactory::GetInstance().CreateInstrument(name);
                if (instrument) {
                    if (config) {
                        instrument->DeserializeFromLua(config);
                    }
                    instruments.push_back(std::move(instrument));
                } else {
                    std::cerr << "Unknown instrument type: " << type << std::endl;
                }
            }
        }
        
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // Pop instruments table

    std::cout << "Loaded " << instruments.size() << " instruments from config" << std::endl;
    return instruments;
}

bool ConfigManager::ExecuteLuaFile(const std::string& filename) {
    if (luaL_dofile(L, filename.c_str()) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        return false;
    }
    return true;
}