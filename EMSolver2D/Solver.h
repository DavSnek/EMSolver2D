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
public: double dt; // time step
public: double delta; // spatial step
public: double t_end; // End time of simulation
public: double x_dim; // length in x dimension 
public: double y_dim; // length in y dimension
public: int x_len; // amount of cells in X-direction
public: int y_len; // amount of cells in Y-direction
public: int t_len; // amount of time steps
public: std::vector< std::vector<std::vector<cell>>> SimReg; // 3-dim matrix that contains 2d matrix of cells for each time-step

	public: 
	Solver(double dt, double delta, double t_end, double xdim, double ydim)//, double (*func)(double,int,int)
		: dt(dt), delta(delta), t_end(t_end), x_dim(xdim), y_dim(ydim){
		x_len = int(x_dim / delta);
		y_len = int(y_dim / delta);
		t_len = int(t_end / dt);
		std::vector<std::vector<cell>> tmpx;
		std::vector<cell> tmpy;
		for (double t = 0; t <= t_end; t += dt)
		{
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
		}
	}
public:
	void updateExplicitTM() //Transverse Magnetic field LeapFrog scheme
	{
		for (int t = 1; t < t_len; t++)
		{
			double current_time = (t - 1) * dt;
			for (int y = 0; y < y_len; y++)
				SimReg[t - 1][10][y].Hz = sin(current_time / 5);
			//Update Hx
			for (int x = 0; x < x_len; x++)
			{
				for (int y = 1; y < y_len - 1; y++)
				{
					SimReg[t][x][y].Hx = SimReg[t - 1][x][y].Hx 
						- dt / (SimReg[t - 1][x][y].mu * delta) * (SimReg[t - 1][x][y+1].Ez - SimReg[t - 1][x][y].Ez);
				}
			}
			//Update Hy
			for (int x = 1; x < x_len - 1; x++)
			{
				for (int y = 0; y < y_len; y++)
				{
					SimReg[t][x][y].Hy = SimReg[t - 1][x][y].Hy 
						+ dt / (SimReg[t - 1][x][y].mu * delta) * (SimReg[t - 1][x+1][y].Ez - SimReg[t - 1][x][y].Ez);
				}
			}
			//Update Ez
			for (int x = 1; x < x_len - 1; x++)
			{
				for (int y = 1; y < y_len - 1; y++)
				{
					SimReg[t][x][y].Ez = SimReg[t - 1][x][y].Ez
						- dt / (SimReg[t - 1][x][y].eps * delta) * (SimReg[t - 1][x][y].Hx - SimReg[t - 1][x][y + 1].Ex)
						+ dt / (SimReg[t - 1][x][y].eps * delta) * (SimReg[t - 1][x][y].Hy - SimReg[t - 1][x - 1][y].Ey); 
				}
			}
		}
	}
public:
	void updateExplicitTE() //Transverse Electric field LeapFrog scheme
	{
		for (int t = 1 ; t < t_len; t++)
		{
			double current_time = (t-1) * dt;
			for(int y = 0;y<y_len;y++)
				SimReg[t-1][10][y].Hz = sin(current_time / 5);
			//Update Ex
			for (int x = 0; x < x_len; x++)
			{
				for (int y = 1; y < y_len-1; y++)
				{
					SimReg[t][x][y].Ex = SimReg[t - 1][x][y].Ex 
						+ dt/ (SimReg[t - 1][x][y].eps*delta) * (SimReg[t - 1][x][y].Hz - SimReg[t - 1][x][y-1].Hz);
				}
			}
			//Update Ey
			for (int x = 1; x < x_len-1; x++)
			{
				for (int y = 0; y < y_len ; y++)
				{
					SimReg[t][x][y].Ey = SimReg[t - 1][x][y].Ey 
						- dt/(SimReg[t - 1][x][y].eps*delta) * (SimReg[t - 1][x][y].Hz - SimReg[t - 1][x-1][y].Hz);
				}
			}
			//Update Hz
			for (int x = 1; x < x_len-1; x++)
			{
				for (int y = 1; y < y_len-1; y++)
				{
					SimReg[t][x][y].Hz = SimReg[t - 1][x][y].Hz 
						+ dt/(SimReg[t - 1][x][y].mu * delta)*(SimReg[t - 1][x+1][y+1].Ex - SimReg[t - 1][x+1][y].Ex) 
						- dt/(SimReg[t - 1][x][y].mu * delta) * (SimReg[t - 1][x+1][y+1].Ey - SimReg[t - 1][x][y+1].Ey);
 				}
			}
		}
	}


};
