#include "cImagePanel.h"

BEGIN_EVENT_TABLE(cImagePanel, wxPanel)
// some useful events
/*
 EVT_MOTION(wxImagePanel::mouseMoved)
 EVT_LEFT_DOWN(wxImagePanel::mouseDown)
 EVT_LEFT_UP(wxImagePanel::mouseReleased)
 EVT_RIGHT_DOWN(wxImagePanel::rightClick)
 EVT_LEAVE_WINDOW(wxImagePanel::mouseLeftWindow)
 EVT_KEY_DOWN(wxImagePanel::keyPressed)
 EVT_KEY_UP(wxImagePanel::keyReleased)
 EVT_MOUSEWHEEL(wxImagePanel::mouseWheelMoved)
 */

 // catch paint events
    EVT_PAINT(cImagePanel::paintEvent)
    //Size event
    EVT_SIZE(cImagePanel::OnSize)
    END_EVENT_TABLE()


    // some useful events
    /*
     void wxImagePanel::mouseMoved(wxMouseEvent& event) {}
     void wxImagePanel::mouseDown(wxMouseEvent& event) {}
     void wxImagePanel::mouseWheelMoved(wxMouseEvent& event) {}
     void wxImagePanel::mouseReleased(wxMouseEvent& event) {}
     void wxImagePanel::rightClick(wxMouseEvent& event) {}
     void wxImagePanel::mouseLeftWindow(wxMouseEvent& event) {}
     void wxImagePanel::keyPressed(wxKeyEvent& event) {}
     void wxImagePanel::keyReleased(wxKeyEvent& event) {}
     */


    cImagePanel::cImagePanel(wxWindow* parent, wxString file, wxBitmapType format) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(400, 200), wxBG_STYLE_TRANSPARENT)
{
    // load the file... ideally add a check to see if loading was successful
    image.LoadFile(file, format);
    w = image.GetWidth();
    h = image.GetHeight();


    /*
    wxRect rect;
    rect.x = 0;
    rect.width = w;
    rect.y = 0;
    rect.height = h;
    this->SetClientSize(rect);
    */
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void cImagePanel::paintEvent(wxPaintEvent& evt)
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
void cImagePanel::paintNow()
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
void cImagePanel::render(wxDC& dc)
{
    int neww, newh;
    dc.GetSize(&neww, &newh);
    double ratio = (double)neww / (double)newh;

    if (neww != w || newh != h)
    {
        resized = wxBitmap(image.Scale(neww, newh /*newh*/, wxIMAGE_QUALITY_HIGH));
        w = neww;
        h = newh * ratio/*newh*/;
        dc.DrawBitmap(resized, 0, 0, false);
    }
    else {
        dc.DrawBitmap(resized, 0, 0, false);
    }
}

/*
 * Here we call refresh to tell the panel to draw itself again.
 * So when the user resizes the image panel the image should be resized too.
 */
void cImagePanel::OnSize(wxSizeEvent& event) {
    Refresh();
    //skip the event.
    event.Skip();
}