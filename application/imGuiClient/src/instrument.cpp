/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     19/06/2026
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "instrument.h"

extern "C" {

InstrumentStatus instrument_start(InstrumentHandle handle) {
    if (!handle) return INSTRUMENT_ERROR_INIT;
    IInstrument* instrument = static_cast<IInstrument*>(handle);
    return instrument->Start();
}

InstrumentStatus instrument_configure(InstrumentHandle handle, const char* config) {
    if (!handle) return INSTRUMENT_ERROR_INIT;
    IInstrument* instrument = static_cast<IInstrument*>(handle);
    return instrument->Configure(config);
}

InstrumentStatus instrument_stop(InstrumentHandle handle) {
    if (!handle) return INSTRUMENT_ERROR_INIT;
    IInstrument* instrument = static_cast<IInstrument*>(handle);
    return instrument->Stop();
}

InstrumentStatus instrument_read(InstrumentHandle handle, void* data, size_t size) {
    if (!handle || !data) return INSTRUMENT_ERROR_IO;
    IInstrument* instrument = static_cast<IInstrument*>(handle);
    return instrument->Read(data, size);
}

InstrumentStatus instrument_write(InstrumentHandle handle, const void* data, size_t size) {
    if (!handle || !data) return INSTRUMENT_ERROR_IO;
    IInstrument* instrument = static_cast<IInstrument*>(handle);
    return instrument->Write(data, size);
}

}