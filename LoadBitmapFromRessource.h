#pragma once
#include <wx/wx.h>
#include <wx/mstream.h>
#include <vector>
#include <string>

wxBitmap* CreateBitmapFromPngResource(const wxString& t_name);
bool LoadDataFromResource(char*& t_data, DWORD& t_dataSize, const wxString& t_name);
wxBitmap* GetBitmapFromMemory(const char* t_data, const DWORD t_size);