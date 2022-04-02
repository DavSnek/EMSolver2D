#pragma once
#include <cmath>
#include <vector>

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
