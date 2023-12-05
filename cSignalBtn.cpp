#include "cSignalBtn.h"

BEGIN_EVENT_TABLE(wxCustomButton, wxPanel)

EVT_MOTION(wxCustomButton::mouseMoved)
EVT_LEFT_DOWN(wxCustomButton::mouseDown)
EVT_LEFT_UP(wxCustomButton::mouseReleased)
EVT_RIGHT_DOWN(wxCustomButton::rightClick)
EVT_LEAVE_WINDOW(wxCustomButton::mouseLeftWindow)
EVT_KEY_DOWN(wxCustomButton::keyPressed)
EVT_KEY_UP(wxCustomButton::keyReleased)
EVT_MOUSEWHEEL(wxCustomButton::mouseWheelMoved)
EVT_COLOURPICKER_CHANGED(IDCPICKER, wxCustomButton::OnPickerChanged)

// catch paint events
EVT_PAINT(wxCustomButton::paintEvent)

END_EVENT_TABLE()

#include "cObjectmanager.h"
#include "cPlot.h"

wxCustomButton::wxCustomButton(wxFrame* parent, wxWindowID id, wxString name, wxString address, wxColor color) :
    wxWindow(parent, id)
{
    wxBoxSizer* h_sizer0 = new wxBoxSizer(wxHORIZONTAL);

    wxButton* sinebtn = new wxButton(this, wxID_ANY, "", wxDefaultPosition, this->FromDIP(wxDefaultSize), wxNO_BORDER);
    sinebtn->SetBitmap(wxBitmap(wxT("SINUSPNG"), wxBITMAP_TYPE_PNG_RESOURCE));

    txt = new wxStaticText(this, wxID_ANY, name, wxDefaultPosition, this->FromDIP(wxDefaultSize), wxTE_CENTRE | wxNO_BORDER);

    picker = new wxColourPickerCtrl(this, IDCPICKER, *wxLIGHT_GREY, wxDefaultPosition,
        this->FromDIP(wxSize(50, 15)), wxCLRP_DEFAULT_STYLE,
        wxDefaultValidator,
        wxString(""));

    h_sizer0->Add(sinebtn, 0, wxEXPAND);
    h_sizer0->Add(txt, 0, wxALIGN_CENTER_VERTICAL);
    h_sizer0->AddStretchSpacer();
    h_sizer0->Add(picker, 0, wxALIGN_CENTER_VERTICAL);
    h_sizer0->AddStretchSpacer();

    wxBoxSizer* h_sizer1 = new wxBoxSizer(wxHORIZONTAL);

    staticunit = new wxStaticText(this, wxID_ANY, "Unit: ", wxDefaultPosition, this->FromDIP(wxDefaultSize), wxTE_CENTRE | wxNO_BORDER);
    unit = new wxStaticText(this, wxID_ANY, "Volt", wxDefaultPosition, this->FromDIP(wxDefaultSize), wxTE_CENTRE | wxNO_BORDER);

    h_sizer1->Add(staticunit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxLEFT, this->FromDIP(10));
    h_sizer1->Add(unit, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);

    wxBoxSizer* h_sizer2 = new wxBoxSizer(wxHORIZONTAL);

    staticaddr = new wxStaticText(this, wxID_ANY, "Physical addr: ", wxDefaultPosition, this->FromDIP(wxDefaultSize), wxTE_CENTRE | wxNO_BORDER);
    addr = new wxStaticText(this, wxID_ANY, address, wxDefaultPosition, this->FromDIP(wxDefaultSize), wxTE_CENTRE | wxNO_BORDER);

    h_sizer2->Add(staticaddr, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxLEFT, this->FromDIP(10));
    h_sizer2->Add(addr, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);

    wxBoxSizer* h_sizer3 = new wxBoxSizer(wxHORIZONTAL);
    min = new wxStaticText(this, wxID_ANY, "Min: 0", wxDefaultPosition, this->FromDIP(wxSize(20, 20)), wxTE_CENTRE | wxNO_BORDER);
    avg = new wxStaticText(this, wxID_ANY, "Average: 0", wxDefaultPosition, this->FromDIP(wxSize(20, 20)), wxTE_CENTRE | wxNO_BORDER);
    max = new wxStaticText(this, wxID_ANY, "Max: 0", wxDefaultPosition, this->FromDIP(wxSize(20, 20)), wxTE_CENTRE | wxNO_BORDER);
    reset = new wxButton(this, IDCRESET_STAT, "", wxPoint(0, 0), this->FromDIP(wxSize(10, 10)), wxNO_BORDER);
    reset->SetBitmap(wxBitmap(wxT("BACKARROWPNG"), wxBITMAP_TYPE_PNG_RESOURCE));
    this->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &wxCustomButton::reset_stat, this, IDCRESET_STAT);


    h_sizer3->Add(min, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    h_sizer3->Add(avg, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    h_sizer3->Add(max, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    h_sizer3->Add(reset, 1, wxEXPAND);



    wxBoxSizer* h_sizer4 = new wxBoxSizer(wxHORIZONTAL);
    show = new wxCheckBox(this, IDCSHOWSIGNAL, "Show", wxDefaultPosition, this->FromDIP(wxSize(100, 20)), wxTE_CENTRE | wxCHK_2STATE);
    show->SetValue(true);
    this->Bind(wxEVT_CHECKBOX, &wxCustomButton::OnShowSignalChanged, this, IDCSHOWSIGNAL);

    h_sizer4->Add(show, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxLEFT, this->FromDIP(10));

    wxStaticLine* separator0 = new wxStaticLine(this,
        wxID_ANY,
        wxDefaultPosition,
        this->FromDIP(wxSize(50, 2)),
        wxLI_HORIZONTAL,
        wxStaticLineNameStr
    );

    wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);
    v_sizer->Add(h_sizer0, 1, wxEXPAND);
    v_sizer->Add(separator0, 1, wxEXPAND);
    v_sizer->Add(h_sizer1, 1, wxEXPAND);
    v_sizer->Add(h_sizer2, 1, wxEXPAND);
    v_sizer->Add(h_sizer3, 1, wxEXPAND);
    v_sizer->Add(h_sizer4, 1, wxEXPAND);

    this->SetSizer(v_sizer);
    SetMinSize(this->FromDIP(wxSize(buttonWidth, buttonHeight)));
    this->name_ = name;
    this->address_ = address;
    this->min_ = 0.0;
    this->avg_ = 0.0;
    this->max_ = 0.0;
    pressedDown = false;
}

void wxCustomButton::set_name(wxString name)
{
    this->txt->SetLabel(name);
    this->name_ = name;
}

void wxCustomButton::set_address(wxString address)
{
    this->addr->SetLabel(address);
    this->address_ = address;
}

void wxCustomButton::set_unit(wxString unit)
{
    this->unit->SetLabel(unit);
    this->unit_ = unit;
}

void wxCustomButton::set_min(double val)
{
    this->min->SetLabel(wxString::Format(wxT("min: %.2lf"), val));
    Layout();
    this->min_ = val;
}
void wxCustomButton::set_average(double val)
{
    this->avg->SetLabel(wxString::Format(wxT("avg: %.2lf"), val));
    Layout();
    this->avg_ = val;
}
void wxCustomButton::set_max(double val)
{
    this->max->SetLabel(wxString::Format(wxT("max: %.2lf"), val));
    Layout();
    this->max_ = val;
}
void wxCustomButton::set_color(wxColor color)
{
    this->picker->SetColour(color);
    this->color_ = color;
}

void wxCustomButton::set_visible(bool isDisplayed)
{
    show->SetValue(isDisplayed);
}

wxString wxCustomButton::get_name()
{
    return this->name_;
}

wxString wxCustomButton::get_address()
{
    return this->address_;
}

double wxCustomButton::get_min()
{
    return this->min_;
}

double wxCustomButton::get_average()
{
    return this->avg_;
}

double wxCustomButton::get_max()
{
    return this->max_;
}

wxColor wxCustomButton::get_color()
{
    return this->picker->GetColour();
}


/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void wxCustomButton::paintEvent(wxPaintEvent& evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void wxCustomButton::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void wxCustomButton::render(wxDC& dc)
{
    //if (pressedDown)
        //dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    //else
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    //dc.SetBrush(*wxGREY_BRUSH);


//dc.DrawRectangle(0, 0, buttonWidth, buttonHeight);

}

void wxCustomButton::OnPickerChanged(wxColourPickerEvent& event)
{
    std::cout << "cObjectmanager->getInstance()\n";
    cObjectmanager* obj_manager = obj_manager->getInstance();

    cPlot* m_plot = obj_manager->get_plot();

    m_plot->update_chan_color(picker->GetColour(), this->GetId());
    event.Skip();
}

void wxCustomButton::OnShowSignalChanged(wxEvent& event)
{
    std::cout << "cObjectmanager->getInstance()\n";
    cObjectmanager* obj_manager = obj_manager->getInstance();

    cPlot* m_plot = obj_manager->get_plot();

    bool isChecked = show->GetValue();
    m_plot->set_signal_visible(isChecked, this->GetId());
    event.Skip();
}

void wxCustomButton::mouseDown(wxMouseEvent& event)
{
    pressedDown = true;
    paintNow();
}
void wxCustomButton::mouseReleased(wxMouseEvent& event)
{
    pressedDown = false;
    paintNow();

    //wxMessageBox(wxT("You pressed a custom button"));
}
void wxCustomButton::mouseLeftWindow(wxMouseEvent& event)
{
    if (pressedDown)
    {
        pressedDown = false;
        paintNow();
    }
}

// currently unused events
void wxCustomButton::mouseMoved(wxMouseEvent& event) {}
void wxCustomButton::mouseWheelMoved(wxMouseEvent& event) {}
void wxCustomButton::rightClick(wxMouseEvent& event) {}
void wxCustomButton::keyPressed(wxKeyEvent& event) {}
void wxCustomButton::keyReleased(wxKeyEvent& event) {}

void wxCustomButton::reset_stat(wxEvent& event)
{
    std::cout << "cObjectmanager->getInstance()\n";
    cObjectmanager* obj_manager = obj_manager->getInstance();

    cPlot* m_plot = obj_manager->get_plot();

    m_plot->reset_chan_statistic_labels(this->GetId());
    event.Skip();
}