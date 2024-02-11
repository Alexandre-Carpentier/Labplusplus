#include "cDeviceBuilder.h"

#pragma comment (lib, "Kernel32.lib")



void DeviceBuilder1::ProduceProtocol(SCPIMODE mode) const
{
    if (mode == SCPIMODE::COM)
    {
        this->product->protocol = std::make_unique <plug_cCom>();
    }
    if (mode == SCPIMODE::TCP)
    {
        this->product->protocol = std::make_unique <plug_cTcp>();
    }
    if (mode == SCPIMODE::USB)
    {
        this->product->protocol = std::make_unique <plug_cUsb>();
    }
    if (mode == SCPIMODE::DAQMX)
    {
        this->product->protocol = std::make_unique <plug_cDaq>();
    }
    if (mode == SCPIMODE::VISA)
    {
        this->product->protocol = std::make_unique <plug_cVisa>();
    }
}

void DeviceBuilder1::ProducePanel(wxWindow* inst) const
{
    this->product->panel = new wxScrolled <wxPanel>(inst, wxID_ANY, wxDefaultPosition, inst->FromDIP(wxSize(400, 600)));
    this->product->panel->FitInside();
    this->product->panel->SetScrollRate(0, 40);
    this->product->header_v_sizer = new wxBoxSizer(wxVERTICAL);
    this->product->header_h_sizer = new wxBoxSizer(wxHORIZONTAL);
    this->product->grid_sizer = new wxFlexGridSizer(30, 2, 10, inst->FromDIP(50));
}

void DeviceBuilder1::ProduceImage(wxWindow* inst) const
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

    std::wstring img_file = L"ADDON\\";
    img_file.append(szFileName);
    img_file.append(L".png");

    wxInitAllImageHandlers();

    this->product->instr_img = new wxStaticBitmap(this->product->panel, wxID_ANY, wxBitmap(img_file.c_str(), wxBITMAP_TYPE_PNG), inst->FromDIP(wxPoint(50, 20)), inst->FromDIP(wxSize(740, 420)));
    //this->product->header = new wxStaticText(this->product->panel, wxID_ANY, szFileName, wxDefaultPosition, inst->FromDIP(wxSize(300, 200)), wxNO_BORDER);
    //this->product->header->SetFont(this->product->header->GetFont().Scale(inst->FromDIP(4)));
    this->product->header_h_sizer->Add(this->product->instr_img, 0, wxEXPAND, inst->FromDIP(20));
   // this->product->header_h_sizer->Add(this->product->header, 0, wxALL, inst->FromDIP(20));
}

void DeviceBuilder1::AddPanelCtrl(CONTROLTYPE type, int length, int height, std::string label)const
{
    wxControl* ctrl = nullptr;

    if (type == CONTROLTYPE::BUT)
    {
        ctrl = new wxButton(this->product->panel, wxID_ANY, label.c_str(), wxDefaultPosition, wxSize(length, height), wxSUNKEN_BORDER);
    }
    if (type == CONTROLTYPE::TXT)
    {
        ctrl = new wxStaticText(this->product->panel, wxID_ANY, label.c_str(), wxDefaultPosition, wxSize(length, height), wxNO_BORDER);
    }
    if (type == CONTROLTYPE::TXTFIELD)
    {
        ctrl = new wxTextCtrl(this->product->panel, wxID_ANY, label.c_str(), wxDefaultPosition, wxSize(length, height), wxTE_MULTILINE | wxTE_RICH | wxHSCROLL | wxSUNKEN_BORDER);
    }
    if (type == CONTROLTYPE::SPINBUT)
    {
        ctrl = new wxSpinCtrl(this->product->panel, wxID_ANY, label.c_str(), wxDefaultPosition, wxSize(length, height), wxNO_BORDER);
    }

    this->product->grid_sizer->Add(ctrl, 1, wxALL, 10);
}