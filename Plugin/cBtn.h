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