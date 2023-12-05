#include "cScaleBtn.h"

BEGIN_EVENT_TABLE(wxScaleButton, wxPanel)

// catch paint events
EVT_PAINT(wxScaleButton::paintEvent)

END_EVENT_TABLE()

#include "cObjectmanager.h"
#include "cPlot.h"

wxScaleButton::wxScaleButton(wxFrame* parent, wxWindowID id) :
    wxWindow(parent, id)
{
    wxBoxSizer* h_sizer0 = new wxBoxSizer(wxHORIZONTAL);

    autoscale = new wxCheckBox(this, IDCAUTOSCALESIGNAL, "Autoscale", wxDefaultPosition, this->FromDIP(wxSize(100, 20)), wxTE_CENTRE | wxCHK_2STATE);
    autoscale->SetValue(true);
    this->Bind(wxEVT_CHECKBOX, &wxScaleButton::OnAutoscaleSignalChanged, this, IDCAUTOSCALESIGNAL);

    staticscale = new wxStaticText(this, wxID_ANY, "Scale factor x1", wxDefaultPosition, this->FromDIP(wxDefaultSize), wxTE_CENTRE | wxNO_BORDER);

    slider = new wxSlider(this,
        IDCSCALESLIDER,
        1,
        1,
        100,
        wxDefaultPosition,
        this->FromDIP(wxSize(400, 20)),
        wxSL_HORIZONTAL,
        wxDefaultValidator,
        "Zoom"
    );

    this->Bind(wxEVT_SLIDER, &wxScaleButton::OnZoomSignalChanged, this, IDCSCALESLIDER);

    h_sizer0->AddStretchSpacer();
    h_sizer0->Add(autoscale, 0, wxEXPAND);
    h_sizer0->AddStretchSpacer();
    h_sizer0->Add(staticscale, 0);
    h_sizer0->Add(slider, 0);
    h_sizer0->AddStretchSpacer();

    SetSizer(h_sizer0);
}


/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void wxScaleButton::paintEvent(wxPaintEvent& evt)
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
void wxScaleButton::paintNow()
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
void wxScaleButton::render(wxDC& dc)
{
    //if (pressedDown)
        //dc.SetBrush(*wxLIGHT_GREY_BRUSH);
    //else
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    //dc.SetBrush(*wxGREY_BRUSH);


//dc.DrawRectangle(0, 0, buttonWidth, buttonHeight);

}

void wxScaleButton::slider_reset()
{
    set_scale_factor(1);
    std::cout << "Zoom: " << 1 << "\n";
    wxString scaletxt = wxString::Format(wxT("Scale factor: x%i "), 1);;
    staticscale->SetLabelText(scaletxt);
    
    slider->SetValue(1);

    this->Layout();
}

void wxScaleButton::set_scale_factor(int factor)
{
    std::cout << "cObjectmanager->getInstance()\n";
    cObjectmanager* obj_manager = obj_manager->getInstance();

    cPlot* m_plot = obj_manager->get_plot();

    m_plot->set_zoom(factor);
}

int wxScaleButton::get_scale_factor()
{
    return 0;
}

void wxScaleButton::OnAutoscaleSignalChanged(wxEvent& event)
{
    std::cout << "cObjectmanager->getInstance()\n";
    cObjectmanager* obj_manager = obj_manager->getInstance();

    cPlot* m_plot = obj_manager->get_plot();

    bool isChecked = autoscale->GetValue();
    m_plot->set_autoscale(isChecked);
    event.Skip();
}

void wxScaleButton::OnZoomSignalChanged(wxCommandEvent& event)
{
    int slider_pos = event.GetInt();
    set_scale_factor(slider_pos);
    std::cout << "Zoom: " << slider_pos << "\n";
    wxString scaletxt = wxString::Format(wxT("Scale factor: x%i "), slider_pos);;
    staticscale->SetLabelText(scaletxt);
    this->Layout();
    event.Skip();
}
