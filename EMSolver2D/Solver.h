﻿#pragma once
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
	double Ez;	 // Z-component of Electric field
	double Hx;	 // X-component of Magnetic field
	double Hy;	 // Y-component of Magnetic field
	double Hz;	 // Z-component of Magnetic field
	double eps;  // Relative Permittivity
	double mu;   // Relative Permability
	double sigE; //	Electric conductivity
	double sigH; // Magnetic condictivity

	cell() : Ex(0), Ey(0), Ez(0), Hx(0), Hy(0), Hz(0), eps(1), mu(1), sigE(0), sigH(0) {}
};

class  Solver
{
public: double cfl; // the Courant-Friedrichs-Levy coefficient
public: double dt; // time step
public: double delta; // spatial step
public: double t_end = 0; // End time step of simulation
public: int t_last = 0; // Last time step of simulation 
public: double x_dim; // length in x dimension 
public: double y_dim; // length in y dimension
public: int x_len; // amount of cells in X-direction
public: int y_len; // amount of cells in Y-direction
public: std::vector< std::vector<std::vector<cell>>> SimReg; // 3-dim matrix that contains 2d matrix of cells for each time-step

	public: 
	Solver(double delta, double cfl, double xdim, double ydim)//, double (*func)(double,int,int)
		: cfl(cfl), delta(delta), t_end(0), x_dim(xdim), y_dim(ydim){
		x_len = int(x_dim / delta);
		y_len = int(y_dim / delta);
		dt = cfl * delta;

		//init matrix for time = 0:
		initNewTimeMatrix();
		t_last--;
		//init matrix for time = dt:
		initNewTimeMatrix();
	}
public:
	void initNewTimeMatrix()
	{
		//init matrix for new time step:
		std::vector<std::vector<cell>> tmpx;
		std::vector<cell> tmpy;
		tmpx.clear();
		for (int x = 0; x < x_len; x++)
		{
			tmpy.clear();
			for (int y = 0; y < y_len; y++)
			{
				tmpy.push_back(cell());
			}
			tmpx.push_back(tmpy);
		}
		SimReg.push_back(tmpx);
		

		for (int y = 0; y < y_len; y++)
			for (int x = 0; x < 2; x++) {
				SimReg[t_last][x][y].Hz = sin(t_end); // First define source in previous step then increment t_last
			}
		t_last ++;
		t_end += dt;
	}
public:
	void testschemeTM()
	{
		initNewTimeMatrix();
		double a = dt / delta;
		for (int x = 1; x < x_len - 1; x++)
		{
			for (int y = 1; y < y_len - 1; y++)
			{
				//Update Hx
				SimReg[t_last][x][y].Hx = SimReg[t_last - 2][x][y].Hx
					- a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x][y + 1].Ez - SimReg[t_last - 1][x][y - 1].Ez)
					- 2 * dt / SimReg[t_last - 1][x][y].mu * SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hx;

				//Update Hy
				SimReg[t_last][x][y].Hy = SimReg[t_last - 2][x][y].Hy
					+ a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x + 1][y].Ez - SimReg[t_last - 1][x - 1][y].Ez)
					- 2 * dt / SimReg[t_last - 1][x][y].mu * SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hy;

				//Update Ez:
				SimReg[t_last][x][y].Ez = SimReg[t_last - 2][x][y].Ez
					+ a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x+1][y].Hy - SimReg[t_last - 1][x - 1][y].Hy)
					- a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x][y+1].Hx - SimReg[t_last - 1][x][y - 1].Hx)
					- 2 * dt / SimReg[t_last - 1][x][y].eps * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ez;
			}
		}
	}
public:
	void testschemeTE()
	{
		initNewTimeMatrix();
		double a = dt / delta;
		for (int x = 1; x < x_len - 1; x++)
		{
			for (int y = 1; y < y_len - 1; y++)
			{
				//Update Ex
				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex
					+ a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz)
					- 2 * dt / SimReg[t_last - 1][x][y].eps * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex;

				//Update Ey
				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
					- a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz)
					- 2 * dt / SimReg[t_last - 1][x][y].eps * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey;

				//Update Hz:
				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
					+ a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex)
					- a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey)
					- 2 * dt / SimReg[t_last - 1][x][y].mu * SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz;
			}
		}
	}

};
