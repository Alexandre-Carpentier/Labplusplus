#include "LoadBitmapFromRessource.h"

//https://wiki.wxwidgets.org/Embedding_PNG_Images#Embedding_data_into_Windows_resource_file

wxBitmap* CreateBitmapFromPngResource(const wxString& t_name)
{
    wxBitmap* r_bitmapPtr = 0;

    char* a_data = 0;
    DWORD       a_dataSize = 0;

    if (LoadDataFromResource(a_data, a_dataSize, t_name))
    {
        r_bitmapPtr = GetBitmapFromMemory(a_data, a_dataSize);
    }

    return r_bitmapPtr;
}

bool LoadDataFromResource(char*& t_data, DWORD& t_dataSize, const wxString& t_name)
{
    bool     r_result = false;
    HGLOBAL  a_resHandle = 0;
    HRSRC    a_resource;

    a_resource = FindResource(0, t_name.wchar_str(), RT_RCDATA);

    if (0 != a_resource)
    {
        a_resHandle = LoadResource(NULL, a_resource);
        if (0 != a_resHandle)
        {
            t_data = (char*)LockResource(a_resHandle);
            t_dataSize = SizeofResource(NULL, a_resource);
            r_result = true;
        }
    }

    return r_result;
}

wxBitmap* GetBitmapFromMemory(const char* t_data, const DWORD t_size)
{
    wxMemoryInputStream a_is(t_data, t_size);
    return new wxBitmap(wxImage(a_is, wxBITMAP_TYPE_PNG, -1), -1);
}