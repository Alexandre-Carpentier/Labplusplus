/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     19/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

#ifdef __cplusplus
extern "C" {
#endif

// Interface C pour les instruments
typedef void* InstrumentHandle;

typedef enum {
    INSTRUMENT_OK = 0,
    INSTRUMENT_ERROR_INIT = -1,
    INSTRUMENT_ERROR_CONFIG = -2,
    INSTRUMENT_ERROR_IO = -3
} InstrumentStatus;

InstrumentStatus instrument_start(InstrumentHandle handle);
InstrumentStatus instrument_configure(InstrumentHandle handle, const char* config);
InstrumentStatus instrument_stop(InstrumentHandle handle);
InstrumentStatus instrument_read(InstrumentHandle handle, void* data, size_t size);
InstrumentStatus instrument_write(InstrumentHandle handle, const void* data, size_t size);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
// Interface C++ pour les instruments
class IInstrument {
public:
    virtual ~IInstrument() = default;

    virtual const char* GetName() const = 0;
    virtual void Render() = 0;
    
    virtual InstrumentStatus Start() = 0;
    virtual InstrumentStatus Configure(const char* config) = 0;
    virtual InstrumentStatus Stop() = 0;
    virtual InstrumentStatus Read(void* data, size_t size) = 0;
    virtual InstrumentStatus Write(const void* data, size_t size) = 0;
};
#endif