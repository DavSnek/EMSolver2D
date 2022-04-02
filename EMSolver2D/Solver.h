#pragma once
#include <cmath>
#include <vector>
// Ex,Ey,Hx,Hy -> macierze o wymiarach symulacji
// eps -> macierz okreslajaca wzgl�dn� przenikalnosc dielektryczna w kazdym punkcie przestrzeni.
// sigma -> macierz okre�laj�ca przewodno�� elektyczn� w ka�dym punkcji przestrzeni. (do symulowania stratnych materia��w)
// funkcja opisujaca zrodlo
// warunki brzegowe - okreslanie i obliczanie. PML, PEC, przenikalnosc dielektryczna

using namespace System;
using namespace System::Drawing;

void changeColor(System::Drawing::Bitmap^ bmp, int cX, int cY)
{
	int sizeX = bmp->Width;
	int sizeY = bmp->Height;
	for (int i = 0; i < sizeX; i++)
		for (int j = 0; j < sizeY; j++)
			bmp->SetPixel(i, j, System::Drawing::Color::FromArgb(255, int(abs(i - cX) * 255 / sizeX), int(abs(j - cY) * 255 / sizeY), 0));
}
struct cell
{
	double Ex;	 // X-component of Electric field
	double Ey;	 // Y-component of Electric field
	double Hx;	 // X-component of Magnetic field
	double Hy;	 // Y-component of Magnetic field
	double eps;  // Relative Permittivity
	double mu;   // Relative Permability
	double sigE; //	Electric conductivity
	double sigH; // Magnetic condictivity
};

class  Solver
{
	std::vector<std::vector<cell>> SimReg;
};
