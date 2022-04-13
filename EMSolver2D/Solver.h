#pragma once
#include <cmath>
#include <vector>
#include <string>

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
	float Ex;	 // X-component of Electric field
	float Ey;	 // Y-component of Electric field
	float Ez;	 // Z-component of Electric field
	float Hx;	 // X-component of Magnetic field
	float Hy;	 // Y-component of Magnetic field
	float Hz;	 // Z-component of Magnetic field
	float eps;  // Relative Permittivity
	float mu;   // Relative Permability
	float sigE; // Electric conductivity
	float sigH; // Magnetic condictivity

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
public: std::vector<std::vector<double>> alpha; // for PML

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

		// initialize alpha matrix (for PML)
		std::vector<double> tmp;
		for (int x = 0; x < x_len; x++)
		{	
			tmp.clear();
			for (int y = 0; y < y_len; y++)
			{
				tmp.push_back(1);
			}
			alpha.push_back(tmp);
		}
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < y_len; j++)
			{
				alpha[i][j] = (i + 1) / 10;
				alpha[x_len - 1 - i][j] = (i + 1) / 10;
			}
		for (int j = 0; j < 10; j++)
			for (int i = 0; i < x_len; i++)
			{
				alpha[i][j] = (j + 1) / 10;
				alpha[i][y_len - 1 - j] = (j + 1) / 10;
			}
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
		//sourceOnePoint();
		t_last ++;
		t_end += dt;
	}
public:
	void sourceTwoPoint()
	{
		for (int y = int(3 * y_len / 4 - 2); y < int(3 * y_len / 4 + 2); y++)
			for (int x = int(3 * x_len / 4 - 2); x < int(3 * x_len / 4 + 2); x++) {
				SimReg[t_last][x][y].Hz = sin(t_end);
			}
		for (int y = int(y_len / 4 - 2); y < int(y_len / 4 + 2); y++)
			for (int x = int(x_len / 4 - 2); x < int(x_len / 4 + 2); x++) {
				SimReg[t_last][x][y].Hz = -sin(t_end);
			}
	}
public:
	void sourcePlaneWave(float omega, std::string direction, float pos, bool TE = true)
	{
		int posidx = int(pos / delta);
		if (direction == "x") {
			if (TE) {
				for (int j = 0; j < y_len; j++)
				{
					SimReg[t_last][posidx-1][j].Hz = sin(omega*t_end);
					SimReg[t_last][posidx][j].Hz = sin(omega*t_end);
				}
			}
			else
			{
				for (int j = 0; j < y_len; j++)
				{
					SimReg[t_last][posidx-1][j].Ez = sin(omega * t_end);
					SimReg[t_last][posidx][j].Ez = sin(omega * t_end);
				}
			}
		}
		else if (direction == "y")
		{
			if (TE) {
				for (int i = 0; i < x_len; i++)
				{
					SimReg[t_last][i][posidx - 1].Hz = sin(omega * t_end);
					SimReg[t_last][i][posidx].Hz = sin(omega * t_end);
				}
			}
			else
			{
				for (int i = 0; i < x_len; i++)
				{
					SimReg[t_last][i][posidx - 1].Ez = sin(omega * t_end);
					SimReg[t_last][i][posidx].Ez = sin(omega * t_end);
				}
			}
		}
	}
public:
	void sourceOnePoint()
	{
		double src = sin(3*t_end);
		SimReg[t_last][int(x_len / 2)][int(y_len / 2)].Hz = src;
		SimReg[t_last][int(x_len / 2) + 1][int(y_len / 2)].Hz = src;
		SimReg[t_last][int(x_len / 2)][int(y_len / 2) + 1].Hz = src;
		SimReg[t_last][int(x_len / 2) + 1][int(y_len / 2) + 1].Hz = src;
	}
public:
	void demoYoung()
	{
		sourcePlaneWave(5,"x", 12*0.05);
		ExplicitTE();
		PEC();
		for (int i = -2; i < 3; i++) 
		{
			for (int j = 0; j < int(y_len/2 - 15); j++) {
				SimReg[t_last][(int(x_len / 3) + i)][j].Ex = 0;
				SimReg[t_last][(int(x_len / 3) + i)][j].Ey = 0;
			}
			for (int j = int(y_len / 2 - 5); j < int(y_len / 2 +5); j++) {
				SimReg[t_last][(int(x_len / 3) + i)][j].Ex = 0;
				SimReg[t_last][(int(x_len / 3) + i)][j].Ey = 0;
			}
			for (int j = int(y_len / 2 + 15); j < y_len; j++) {
				SimReg[t_last][(int(x_len / 3) + i)][j].Ex = 0;
				SimReg[t_last][(int(x_len / 3) + i)][j].Ey = 0;
			}
			/*for (int j = 0; j < 95; j++) {
				SimReg[t_last][(int(x_len / 3) + i)][j].Ex = 0;
				SimReg[t_last][(int(x_len / 3) + i)][j].Ey = 0;
			}
			for (int j = 105; j < y_len; j++) {
				SimReg[t_last][(int(x_len / 3) + i)][j].Ex = 0;
				SimReg[t_last][(int(x_len / 3) + i)][j].Ey = 0;
			}*/
		}
	}
public:
	void PEC()
	{
		for (int j = 0; j < 2; j++)
			for (int i = 0; i < x_len; i++)
			{
				SimReg[t_last][i][j].Ex = 0;
				SimReg[t_last][i][y_len - 1 - j].Ex = 0;
				SimReg[t_last][i][j].Ey = 0;
				SimReg[t_last][i][y_len - 1 - j].Ey = 0;
		}
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < y_len; j++)
			{
				SimReg[t_last][i][j].Ex = 0;
				SimReg[t_last][x_len - 1 - i][j].Ex = 0;
				SimReg[t_last][i][j].Ey = 0;
				SimReg[t_last][x_len - 1 - i][j].Ey = 0;
			}
	}
public:
	void primitiveABC()
	{
		float mult = 0.25;
		for (int j = 0; j < 10; j++)
			for (int i = 0; i < x_len; i++)
			{
				SimReg[t_last][i][j].sigE = mult * (10 - j);
				SimReg[t_last][i][j].eps *=  1 + mult * (10 - j);
				SimReg[t_last][i][j].sigH = mult * (10 - j);
				SimReg[t_last][i][j].mu *= 1 + mult * (10 - j);
				SimReg[t_last][i][y_len - 1 - j].sigE = mult * (10 - j);
				SimReg[t_last][i][y_len - 1 - j].eps *= 1 + mult * (10 - j);
				SimReg[t_last][i][y_len - 1 - j].sigH = mult * (10 - j);
				SimReg[t_last][i][y_len - 1 - j].mu *= 1 + mult * (10 - j);
			}
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < y_len; j++)
			{
				SimReg[t_last][i][j].sigE = mult * (10 - i);
				SimReg[t_last][i][j].eps *= 1 + mult * (10 - i);
				SimReg[t_last][i][j].sigH = mult * (10 - i);
				SimReg[t_last][i][j].mu *= 1 + mult * (10 - i);
				SimReg[t_last][x_len - 1 - i][j].sigE = mult * (10 - i);
				SimReg[t_last][x_len - 1 - i][j].eps *= 1 + mult * (10 - i);
				SimReg[t_last][x_len - 1 - i][j].sigH = mult * (10 - i);
				SimReg[t_last][x_len - 1 - i][j].mu *= 1 + mult * (10 - i);
			}

		for (int j = 0; j < 2; j++)
			for (int i = 0; i < x_len; i++)
			{
				SimReg[t_last][i][j].Hz *= 0.2;
				SimReg[t_last][i][j].Ex *= 0.2;
				SimReg[t_last][i][j].Ey *= 0.2;
				SimReg[t_last][i][y_len - 1 - j].Hz *= 0.2;
				SimReg[t_last][i][y_len - 1 - j].Ex *= 0.2;
				SimReg[t_last][i][y_len - 1 - j].Ey *= 0.2;
				//SimReg[t_last][i][j].Ey = 0;
				//SimReg[t_last][i][y_len - 1 - j].Ey = 0;
			}
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < y_len; j++)
			{
				SimReg[t_last][i][j].Hz *= 0.2;
				SimReg[t_last][i][j].Ex *= 0.2;
				SimReg[t_last][i][j].Ey *= 0.2;
				SimReg[t_last][y_len - 1 - i][j].Hz *= 0.2;
				SimReg[t_last][y_len - 1 - i][j].Ex *= 0.2;
				SimReg[t_last][y_len - 1 - i][j].Ey *= 0.2;
				//SimReg[t_last][i][j].Ey = 0;
				//SimReg[t_last][y_len - 1 - i][j].Ey = 0;
			}
	}
public:
	void naiveattenuation()
	{
		float mult = 1;
		for (int j = 0; j < 12; j++)
			for (int i = 0; i < x_len; i++)
			{
				SimReg[t_last][i][j].Ex *= exp(-mult * j);
				SimReg[t_last][i][j].Ey *= exp(-mult * j);
				SimReg[t_last][i][y_len - 1 - j].Ex *= exp(-mult *j);
				SimReg[t_last][i][y_len - 1 - j].Ey *= exp(-mult *j);

				SimReg[t_last - 1][i][j].Ex *= exp(-mult * j);
				SimReg[t_last - 1][i][j].Ey *= exp(-mult * j);
				SimReg[t_last - 1][i][y_len - 1 - j].Ex *= exp(-mult * j);
				SimReg[t_last - 1][i][y_len - 1 - j].Ey *= exp(-mult * j);
			}

		for (int i = 0; i < 12; i++)
			for (int j = 0; j < y_len; j++)
			{ 
				SimReg[t_last][i][j].Ex *= exp(-mult * j);
				SimReg[t_last][i][j].Ey *= exp(-mult * j);
				SimReg[t_last][x_len - 1 - i][j].Ex *= exp(-mult * j);
				SimReg[t_last][x_len - 1 - i][j].Ey *= exp(-mult * j);

				SimReg[t_last - 1][i][j].Ex *= exp(-mult * j);
				SimReg[t_last - 1][i][j].Ey *= exp(-mult * j);
				SimReg[t_last - 1][x_len - 1 - i][j].Ex *= exp(-mult * j);
				SimReg[t_last - 1][x_len - 1 - i][j].Ey *= exp(-mult * j);
			}
	}
public:
	void PML()
	{
		double a = dt / delta;
		double aEx, aEy, bEx, bEy;
		double phiEx = 1;
		double phiEy = 1;

		for (int x = 1; x < 11; x++)
		{
			for (int y = 1; y < y_len - 1; y++)
			{
				aEx = SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEx = exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEy = bEy * 1 + aEy * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz);

				aEy = SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEy = exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEx = bEx * 1 + aEx * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz);
				//Update Ex
				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex
					+ a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz)
					- 2 * dt / SimReg[t_last - 1][x][y].eps * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex
					- dt / SimReg[t_last - 1][x][y].eps * (bEx * x * delta * phiEy + aEx * x * delta * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz) / (2 * dt));

				//Update Ey
				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
					- a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz)
					- 2 * dt / SimReg[t_last - 1][x][y].eps * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey
					- dt / SimReg[t_last - 1][x][y].eps * (bEy * x * delta * phiEx + aEy * y * delta * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz) / (2 * dt));
				//Update Hz:
				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
					+ a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex)
					- a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey)
					- 2 * dt / SimReg[t_last - 1][x][y].mu * SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz;
			}
		}
		for (int x = x_len-11; x < x_len - 1; x++)
		{
			for (int y = 1; y < y_len - 1; y++)
			{
				aEx = SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEx = exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEy = bEy * 1 + aEy * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz);

				aEy = SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEy = exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEx = bEx * 1 + aEx * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz);
				//Update Ex
				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex
					+ a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz)
					- 2 * dt / SimReg[t_last - 1][x][y].eps * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex
					- dt / SimReg[t_last - 1][x][y].eps * (bEx * x * delta * phiEy + aEx * x * delta * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz) / (2 * dt));

				//Update Ey
				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
					- a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz)
					- 2 * dt / SimReg[t_last - 1][x][y].eps * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey
					- dt / SimReg[t_last - 1][x][y].eps * (bEy * x * delta * phiEx + aEy * y * delta * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz) / (2 * dt));
				//Update Hz:
				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
					+ a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex)
					- a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey)
					- 2 * dt / SimReg[t_last - 1][x][y].mu * SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz;
			}
		}
		for (int x = 1; x < x_len - 1; x++)
		{
			for (int y = y_len - 11; y < y_len - 1; y++)
			{
				aEx = SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEx = exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEy = bEy * phiEy + aEy * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz);

				aEy = SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEy = exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEx = bEx * phiEx + aEx * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz);
				//Update Ex
				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex
					+ a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz)
					- 2 * dt / SimReg[t_last - 1][x][y].eps * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex
					- dt / SimReg[t_last - 1][x][y].eps * (bEx * x * delta * phiEy + aEx * x * delta * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz) / (2 * dt));

				//Update Ey
				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
					- a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz)
					- 2 * dt / SimReg[t_last - 1][x][y].eps * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey
					- dt / SimReg[t_last - 1][x][y].eps * (bEy * x * delta * phiEx + aEy * y * delta * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz) / (2 * dt));
				//Update Hz:
				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
					+ a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex)
					- a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey)
					- 2 * dt / SimReg[t_last - 1][x][y].mu * SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz;
			}
		}
		for (int x = 1; x < 10; x++)
		{
			for (int y = 1; y < y_len - 1; y++)
			{
				aEx = SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEx = exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEy = bEy * 1 + aEy * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz);

				aEy = SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEy = exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEx = bEx * 1 + aEx * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz);
				//Update Ex
				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex
					+ a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz)
					- 2 * dt / SimReg[t_last - 1][x][y].eps * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex
					- dt / SimReg[t_last - 1][x][y].eps * (bEx * x * delta * phiEy + aEx * x * delta * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz) / (2 * dt));

				//Update Ey
				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
					- a / SimReg[t_last - 1][x][y].eps * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz)
					- 2 * dt / SimReg[t_last - 1][x][y].eps * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey
					- dt / SimReg[t_last - 1][x][y].eps * (bEy * x * delta * phiEx + aEy * y * delta * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz) / (2 * dt));
				//Update Hz:
				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
					+ a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex)
					- a / SimReg[t_last - 1][x][y].mu * (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey)
					- 2 * dt / SimReg[t_last - 1][x][y].mu * SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz;
			}
		}
	}
public:
	void ExplicitTM() // Leapfrog scheme for time evolution of TM Mode
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
	void ExplicitTE() // Leapfrog scheme for time evolution of TE Mode
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
