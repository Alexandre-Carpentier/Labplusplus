#include "cApp.h"
#include <wx/setup.h>

wxIMPLEMENT_APP_CONSOLE(cApp);
//wxIMPLEMENT_APP(cApp);

bool cApp::OnInit() {

	wxInitAllImageHandlers();
	m_Frame = new cMain();
	m_Frame->SetIcon(wxIcon(wxT("icon.ico"), wxBITMAP_TYPE_ICO));
	m_Frame->Show();
	return true;
}
