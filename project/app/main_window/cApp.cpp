/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#include "../main_window/cApp.h"
#include <wx/wx.h>
#include <wx/setup.h>


wxIMPLEMENT_APP_CONSOLE(cApp);
//wxIMPLEMENT_APP(cApp);

bool cApp::OnInit() 
{
	wxInitAllImageHandlers();
	m_Frame = new cMain();	
	if (m_Frame)
	{
		system("DIR");
		m_Frame->SetIcon(wxIcon(wxT("icon.ico"), wxBITMAP_TYPE_ICO));
		m_Frame->Show();
	}
	return true;
}
