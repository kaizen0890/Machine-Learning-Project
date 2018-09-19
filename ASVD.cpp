// ASVD.cpp : main project file.

#include "stdafx.h"
#include "frm_ASVD.h"

using namespace ASVD;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew frm_ASVD());
	return 0;
}
