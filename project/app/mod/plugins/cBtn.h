/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _BTN_H_
#define _BTN_H_
#include <wx/wx.h>
class cBtn
{
public:
    cBtn()
    {
        btn = new wxButton();
    }
private:
    wxButton* btn = nullptr;
};

#endif