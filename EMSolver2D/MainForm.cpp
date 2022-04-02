#include "MainForm.h"
#include <cmath>
#include <random>
#include <time.h>


using namespace System;
using namespace System::Windows::Forms;


void main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	EMSolver2D::MainForm form;
	Application::Run(% form);
}
