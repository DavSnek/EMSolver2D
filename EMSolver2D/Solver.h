#pragma once
#include <cmath>
#include <vector>
#include <string>
#include "EMConstants.h"
#include "Particle.h"

using namespace System;
using namespace System::Drawing;

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
public: std::vector<Particle*> particles;

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

		t_last++;
		t_end += dt;
	}
public:
	void initNewParticle(float charge, float mass, float posx, float posy, float vx, float vy)
	{
		Particle* tmp = new Particle(charge, mass, posx, posy, vx, vy);
		this->particles.push_back(tmp);
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
	void sourcePlaneWave(float omega, double amplitude, std::string direction, float pos, bool TE = true)
	{
		int posidx = int(pos / delta);
		if (direction == "x") {
			if (TE) {
				for (int j = 0; j < y_len; j++)
				{
					SimReg[t_last][posidx-1][j].Hz = amplitude * sin(omega*t_end);
					SimReg[t_last][posidx][j].Hz = amplitude * sin(omega*t_end);
				}
			}
			else
			{
				for (int j = 0; j < y_len; j++)
				{
					SimReg[t_last][posidx-1][j].Ez = amplitude * sin(omega * t_end);
					SimReg[t_last][posidx][j].Ez = amplitude * sin(omega * t_end);
				}
			}
		}
		else if (direction == "y")
		{
			if (TE) {
				for (int i = 0; i < x_len; i++)
				{
					SimReg[t_last][i][posidx - 1].Hz = amplitude * sin(omega * t_end);
					SimReg[t_last][i][posidx].Hz = amplitude * sin(omega * t_end);
				}
			}
			else
			{
				for (int i = 0; i < x_len; i++)
				{
					SimReg[t_last][i][posidx - 1].Ez = amplitude * sin(omega * t_end);
					SimReg[t_last][i][posidx].Ez = amplitude * sin(omega * t_end);
				}
			}
		}
	}
public:
	void sourceOnePoint(float x, float y, float omega, double amplitude, bool TE = true)
	{
		double src1 = amplitude * sin(6.28 * omega * t_end);
		double src2 = amplitude * sin(6.28 * omega * t_end + delta * 6.28*omega/c);
		if (TE)
		{
			SimReg[t_last][int(x)][int(y)].Hz = src1;

			SimReg[t_last][int(x) + 1][int(y + 1)].Hz = src2;
			SimReg[t_last][int(x) - 1][int(y) - 1].Hz = src2;
			SimReg[t_last][int(x) + 1][int(y) - 1].Hz = src2;
			SimReg[t_last][int(x) + 1][int(y) + 1].Hz = src2;

			SimReg[t_last][int(x)][int(y) + 1].Hz = src2;
			SimReg[t_last][int(x)][int(y) - 1].Hz = src2;
			SimReg[t_last][int(x) + 1][int(y)].Hz = src2;
			SimReg[t_last][int(x) - 1][int(y)].Hz = src2;
		}
		else
		{
			SimReg[t_last][int(x)][int(y)].Ez = src1;

			SimReg[t_last][int(x) + 1][int(y) + 1].Ez = src2;
			SimReg[t_last][int(x) - 1][int(y) - 1].Ez = src2;
			SimReg[t_last][int(x) + 1][int(y) - 1].Ez = src2;
			SimReg[t_last][int(x) + 1][int(y) + 1].Ez = src2;

			SimReg[t_last][int(x)][int(y) + 1].Ez = src2;
			SimReg[t_last][int(x)][int(y) - 1].Ez = src2;
			SimReg[t_last][int(x) + 1][int(y)].Ez = src2;
			SimReg[t_last][int(x) - 1][int(y)].Ez = src2;
		}

	}
public:
	void PEC()
	{
		for (int j = 0; j < 4; j++)
			for (int i = 0; i < x_len; i++)
			{
				SimReg[t_last][i][j].Ex = 0;
				SimReg[t_last][i][y_len - 1 - j].Ex = 0;
				SimReg[t_last][i][j].Ey = 0;
				SimReg[t_last][i][y_len - 1 - j].Ey = 0;
				SimReg[t_last][i][j].Ez = 0;
				SimReg[t_last][i][y_len - 1 - j].Ez = 0;
		}
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < y_len; j++)
			{
				SimReg[t_last][i][j].Ex = 0;
				SimReg[t_last][x_len - 1 - i][j].Ex = 0;
				SimReg[t_last][i][j].Ey = 0;
				SimReg[t_last][x_len - 1 - i][j].Ey = 0;
				SimReg[t_last][i][j].Ez = 0;
				SimReg[t_last][x_len - 1 - i][j].Ez = 0;
			}
	}
public:
	void PBC()
	{
		//Periodic Boundary Conditions
		//for particles:
		for (auto p : particles)
		{
			if (p->pos[0] + p->v[0] * dt >= x_len) p->pos[0] = 1;
			if (p->pos[0] + p->v[0] * dt < 1) p->pos[0] = x_len - 2;
			if (p->pos[1] + p->v[1] * dt >= y_len) p->pos[1] = 1;
			if (p->pos[1] + p->v[1] * dt < 1) p->pos[1] = y_len - 2;
		}
		// for fields:
		for (int x = 0; x < x_len; x++)
		{
			SimReg[t_last][x][0] = SimReg[t_last][x][y_len - 1];
		}
		for (int y = 0; y < y_len; y++)
		{
			SimReg[t_last][0][y] = SimReg[t_last][x_len - 1][y];
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
				aEx = SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEx = exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEy = bEy * 1 + aEy * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz);

				aEy = SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEy = exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEx = bEx * 1 + aEx * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz);
				//Update Ex
				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex
					+ a / (SimReg[t_last - 1][x][y].eps*eps_0) * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz)
					- 2 * dt / (SimReg[t_last - 1][x][y].eps * eps_0) * SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex
					- dt / (SimReg[t_last - 1][x][y].eps * eps_0)* (bEx * x * delta * phiEy + aEx * x * delta * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz) / (2 * dt));

				//Update Ey
				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
					- a / (SimReg[t_last - 1][x][y].eps * eps_0)* (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz)
					- 2 * dt / (SimReg[t_last - 1][x][y].eps * eps_0)* SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey
					- dt / (SimReg[t_last - 1][x][y].eps * eps_0)* (bEy * x * delta * phiEx + aEy * y * delta * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz) / (2 * dt));
				//Update Hz:
				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
					+ a / (SimReg[t_last - 1][x][y].mu * mu_0)* (SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex)
					- a / (SimReg[t_last - 1][x][y].mu * mu_0)* (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey)
					- 2 * dt / (SimReg[t_last - 1][x][y].mu * mu_0)* SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz;
			}
		}
		for (int x = x_len-11; x < x_len - 1; x++)
		{
			for (int y = 1; y < y_len - 1; y++)
			{
				aEx = SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEx = exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEy = bEy * 1 + aEy * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz);

				aEy = SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEy = exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEx = bEx * 1 + aEx * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz);
				//Update Ex
				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex
					+ a / (SimReg[t_last - 1][x][y].eps * eps_0) * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz)
					- 2 * dt / (SimReg[t_last - 1][x][y].eps * eps_0)* SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex
					- dt / (SimReg[t_last - 1][x][y].eps * eps_0)* (bEx * x * delta * phiEy + aEx * x * delta * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz) / (2 * dt));

				//Update Ey
				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
					- a / (SimReg[t_last - 1][x][y].eps * eps_0)* (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz)
					- 2 * dt / (SimReg[t_last - 1][x][y].eps * eps_0)* SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey
					- dt / (SimReg[t_last - 1][x][y].eps * eps_0)* (bEy * x * delta * phiEx + aEy * y * delta * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz) / (2 * dt));
				//Update Hz:
				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
					+ a / (SimReg[t_last - 1][x][y].mu * mu_0)* (SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex)
					- a / (SimReg[t_last - 1][x][y].mu * mu_0)* (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey)
					- 2 * dt / (SimReg[t_last - 1][x][y].mu * mu_0)* SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz;
			}
		}
		for (int x = 1; x < x_len - 1; x++)
		{
			for (int y = y_len - 11; y < y_len - 1; y++)
			{
				aEx = SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEx = exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEy = bEy * phiEy + aEy * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz);

				aEy = SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEy = exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEx = bEx * phiEx + aEx * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz);
				//Update Ex
				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex
					+ a / (SimReg[t_last - 1][x][y].eps * eps_0)* (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz)
					- 2 * dt / (SimReg[t_last - 1][x][y].eps * eps_0)* SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex
					- dt / (SimReg[t_last - 1][x][y].eps * eps_0)* (bEx * x * delta * phiEy + aEx * x * delta * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz) / (2 * dt));

				//Update Ey
				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
					- a / (SimReg[t_last - 1][x][y].eps * eps_0) * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz)
					- 2 * dt / (SimReg[t_last - 1][x][y].eps * eps_0)* SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey
					- dt / (SimReg[t_last - 1][x][y].eps * eps_0)* (bEy * x * delta * phiEx + aEy * y * delta * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz) / (2 * dt));
				//Update Hz:
				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
					+ a / (SimReg[t_last - 1][x][y].mu * mu_0)* (SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex)
					- a / (SimReg[t_last - 1][x][y].mu * mu_0)* (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey)
					- 2 * dt / (SimReg[t_last - 1][x][y].mu * mu_0)* SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz;
			}
		}
		for (int x = 1; x < 10; x++)
		{
			for (int y = 1; y < y_len - 1; y++)
			{
				aEx = SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEx = exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEy = bEy * 1 + aEy * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz);

				aEy = SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE / (SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) * (exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt) - 1);
				bEy = exp(-(SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE + alpha[x][y]) / dt);
				phiEx = bEx * 1 + aEx * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz);
				//Update Ex
				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex
					+ a / (SimReg[t_last - 1][x][y].eps * eps_0)* (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz)
					- 2 * dt / (SimReg[t_last - 1][x][y].eps * eps_0)* SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex
					- dt / (SimReg[t_last - 1][x][y].eps * eps_0)* (bEx * x * delta * phiEy + aEx * x * delta * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz) / (2 * dt));

				//Update Ey
				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
					- a / (SimReg[t_last - 1][x][y].eps * eps_0)* (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz)
					- 2 * dt / (SimReg[t_last - 1][x][y].eps * eps_0)* SimReg[t_last - 1][x][y].mu * mu_0 * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey
					- dt / (SimReg[t_last - 1][x][y].eps * eps_0)* (bEy * x * delta * phiEx + aEy * y * delta * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz) / (2 * dt));
				//Update Hz:
				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
					+ a / (SimReg[t_last - 1][x][y].mu * mu_0)* (SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex)
					- a / (SimReg[t_last - 1][x][y].mu * mu_0)* (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey)
					- 2 * dt / (SimReg[t_last - 1][x][y].mu* mu_0)* SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz;
			}
		}
	}
//public:
//	void ExplicitTM() // Leapfrog scheme for time evolution of TM Mode
//	{
//		initNewTimeMatrix();
//		double a = dt / delta;
//		for (int x = 1; x < x_len - 1; x++)
//		{
//			for (int y = 1; y < y_len - 1; y++)
//			{
//				//Update Hx
//				SimReg[t_last][x][y].Hx = SimReg[t_last - 2][x][y].Hx
//					- a * (SimReg[t_last - 1][x][y + 1].Ez - SimReg[t_last - 1][x][y - 1].Ez) * (SimReg[t_last - 1][x][y].eps * eps_0 * SimReg[t_last - 1][x][y].mu * mu_0)
//					- 2 * dt * SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hx;
//
//				//Update Hy
//				SimReg[t_last][x][y].Hy = SimReg[t_last - 2][x][y].Hy
//					+ a * (SimReg[t_last - 1][x + 1][y].Ez - SimReg[t_last - 1][x - 1][y].Ez) * (SimReg[t_last - 1][x][y].eps * eps_0 * SimReg[t_last - 1][x][y].mu * mu_0)
//					- 2 * SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hy;
//
//				//Update Ez:
//				SimReg[t_last][x][y].Ez = SimReg[t_last - 2][x][y].Ez
//					+ a / (SimReg[t_last - 1][x][y].eps * eps_0 * SimReg[t_last - 1][x][y].mu * mu_0) * (SimReg[t_last - 1][x+1][y].Hy - SimReg[t_last - 1][x - 1][y].Hy)
//					- a / (SimReg[t_last - 1][x][y].eps * eps_0 * SimReg[t_last - 1][x][y].mu * mu_0) * (SimReg[t_last - 1][x][y+1].Hx - SimReg[t_last - 1][x][y - 1].Hx)
//					- 2 * dt / (SimReg[t_last - 1][x][y].eps * eps_0) * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ez;
//			}
//		}
//	}
//public:
//	void ExplicitTE() // Leapfrog scheme for time evolution of TE Mode
//	{
//		initNewTimeMatrix();
//		double a = dt / delta;
//		for (int x = 1; x < x_len - 1; x++)
//		{
//			for (int y = 1; y < y_len - 1; y++)
//			{
//				//Update Ex
//				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex
//					+ a / (SimReg[t_last - 1][x][y].eps * eps_0 * SimReg[t_last - 1][x][y].mu * mu_0) * (SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz)
//					- 2 * dt / (SimReg[t_last - 1][x][y].eps * eps_0) * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex;
//
//				//Update Ey
//				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
//					- a / (SimReg[t_last - 1][x][y].eps * eps_0 * SimReg[t_last - 1][x][y].mu * mu_0) * (SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz)
//					- 2 * dt / (SimReg[t_last - 1][x][y].eps * eps_0) * SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey;
//
//				//Update Hz:
//				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
//					+ (a * (SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex)
//					- a * (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey)) * (SimReg[t_last - 1][x][y].eps * eps_0 * SimReg[t_last - 1][x][y].mu * mu_0)
//					- 2 * dt * SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz;
//			}
//		}
//	}
public:
	void ExplicitTM() // Leapfrog scheme for time evolution of TM Mode // NOT WORKING
	{
		initNewTimeMatrix();
		//double a = dt / delta;
		for (int x = 1; x < x_len - 1; x++)
		{
			for (int y = 1; y < y_len - 1; y++)
			{
				//Update Hx:
				SimReg[t_last][x][y].Hx = SimReg[t_last - 2][x][y].Hx
					- 2 * dt / SimReg[t_last - 1][x][y].mu / mu_0 * ((SimReg[t_last - 1][x][y + 1].Ez - SimReg[t_last - 1][x][y - 1].Ez) / (2 * delta) + SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hx);

				//Update Hy:
				SimReg[t_last][x][y].Hy = SimReg[t_last - 2][x][y].Hy
					+ 2 * dt / SimReg[t_last - 1][x][y].mu / mu_0 * ((SimReg[t_last - 1][x + 1][y].Ez - SimReg[t_last - 1][x - 1][y].Ez) / (2 * delta) - SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hy);

				//Update Ez:
				SimReg[t_last][x][y].Ez = SimReg[t_last - 2][x][y].Ez 
					+ 2 * dt / SimReg[t_last - 1][x][y].eps / eps_0 * ((SimReg[t_last - 1][x + 1][y].Hy - SimReg[t_last - 1][x - 1][y].Hy) / (2 * delta) - (SimReg[t_last - 1][x][y + 1].Hx - SimReg[t_last - 1][x][y - 1].Hx) / (2 * delta) - SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ez);
			}
		}
	}
public:
	void ExplicitTE() // Leapfrog scheme for time evolution of TE Mode //
	{
		initNewTimeMatrix();
		double a = dt / delta;
		for (int x = 1; x < x_len - 1; x++)
		{
			for (int y = 1; y < y_len - 1; y++)
			{
				//Update Ex:
				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex 
					+ 2 * dt / SimReg[t_last - 1][x][y].eps / eps_0 * ((SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz) / (2 * delta) - SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex);

				//Update Ey:
				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
					- 2 * dt / SimReg[t_last - 1][x][y].eps / eps_0 * ((SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz) / (2 * delta) + SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey);

				//Update Hz:
				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
					+ 2 * dt / SimReg[t_last - 1][x][y].mu / mu_0 * ((SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex) / (2 * delta) - (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey) / (2 * delta) - SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz);
			}
		}
	}
public:
	void ExplicitTEM() // Leapfrog scheme for time evolution of TEM Mode //
	{
		initNewTimeMatrix();
		double a = dt / delta;
		for (int x = 1; x < x_len - 1; x++)
		{
			for (int y = 1; y < y_len - 1; y++)
			{

				//Update Hx:
				SimReg[t_last][x][y].Hx = SimReg[t_last - 2][x][y].Hx
					- 2 * dt / SimReg[t_last - 1][x][y].mu / mu_0 * ((SimReg[t_last - 1][x][y + 1].Ez - SimReg[t_last - 1][x][y - 1].Ez) / (2 * delta) + SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hx);

				//Update Hy:
				SimReg[t_last][x][y].Hy = SimReg[t_last - 2][x][y].Hy
					+ 2 * dt / SimReg[t_last - 1][x][y].mu / mu_0 * ((SimReg[t_last - 1][x + 1][y].Ez - SimReg[t_last - 1][x - 1][y].Ez) / (2 * delta) - SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hy);

				//Update Hz:
				SimReg[t_last][x][y].Hz = SimReg[t_last - 2][x][y].Hz
					+ 2 * dt / SimReg[t_last - 1][x][y].mu / mu_0 * ((SimReg[t_last - 1][x][y + 1].Ex - SimReg[t_last - 1][x][y - 1].Ex) / (2 * delta) - (SimReg[t_last - 1][x + 1][y].Ey - SimReg[t_last - 1][x - 1][y].Ey) / (2 * delta) - SimReg[t_last - 1][x][y].sigH * SimReg[t_last - 1][x][y].Hz);

				//Update Ex:
				SimReg[t_last][x][y].Ex = SimReg[t_last - 2][x][y].Ex
					+ 2 * dt / SimReg[t_last - 1][x][y].eps / eps_0 * ((SimReg[t_last - 1][x][y + 1].Hz - SimReg[t_last - 1][x][y - 1].Hz) / (2 * delta) - SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ex);

				//Update Ey:
				SimReg[t_last][x][y].Ey = SimReg[t_last - 2][x][y].Ey
					- 2 * dt / SimReg[t_last - 1][x][y].eps / eps_0 * ((SimReg[t_last - 1][x + 1][y].Hz - SimReg[t_last - 1][x - 1][y].Hz) / (2 * delta) + SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ey);

				//Update Ez:
				SimReg[t_last][x][y].Ez = SimReg[t_last - 2][x][y].Ez
					+ 2 * dt / SimReg[t_last - 1][x][y].eps / eps_0 * ((SimReg[t_last - 1][x + 1][y].Hy - SimReg[t_last - 1][x - 1][y].Hy) / (2 * delta) - (SimReg[t_last - 1][x][y + 1].Hx - SimReg[t_last - 1][x][y - 1].Hx) / (2 * delta) - SimReg[t_last - 1][x][y].sigE * SimReg[t_last - 1][x][y].Ez);
			}
		}
	}
public:
	void PICpos()
	{
		// update positions
		for (auto p : particles)
		{
			p->pos[0] += p->v[0] * dt / delta;
			p->pos[1] += p->v[1] * dt / delta;
		}
	}
public:
	void PICdistribution()
	{
		float tmpE[3];
		float tmpH;
		//update fields from particles
		for (int x = 0; x < x_len; x++)
		{
			for (int y = 0; y < y_len; y++)
			{
				tmpE[0] = 0;
				tmpE[1] = 0;
				tmpE[2] = 0;
				tmpH = 0;
				for (auto p : particles)
				{
					tmpE[0] = p->getE(x, y)[0];
					tmpE[1] = p->getE(x, y)[1];
					tmpE[2] = p->getE(x, y)[2];
					tmpH = p->getH(x, y, SimReg[t_last][x][y].mu);
					this->SimReg[t_last-1][x][y].Ex += tmpE[0];
					this->SimReg[t_last-1][x][y].Ey += tmpE[1];
					this->SimReg[t_last-1][x][y].Ez += tmpE[2];
					this->SimReg[t_last-1][x][y].Hz += tmpH;
					this->SimReg[t_last][x][y].Ex += tmpE[0];
					this->SimReg[t_last][x][y].Ey += tmpE[1];
					this->SimReg[t_last][x][y].Ez += tmpE[2];
					this->SimReg[t_last][x][y].Hz += tmpH;
				}
			}
		}
	}
public:
	void PICvel()
	{
		for (auto p : particles)
		{
			p->v[0] += p->charge / p->mass * dt * SimReg[t_last][int(p->pos[0])][int(p->pos[1])].Ex;
			p->v[1] += p->charge / p->mass * dt * SimReg[t_last][int(p->pos[0])][int(p->pos[1])].Ey;
		}
	}
public:
	void demoYoung()
	{
		sourcePlaneWave(WlenToFrq(600e-9), 1e-20, "x", 12 * 0.05);
		ExplicitTE();
		PEC();
		for (int i = -2; i < 3; i++)
		{
			for (int j = 0; j < int(y_len / 2 - 15); j++) {
				SimReg[t_last][(int(x_len / 3) + i)][j].Ex = 0;
				SimReg[t_last][(int(x_len / 3) + i)][j].Ey = 0;
			}
			for (int j = int(y_len / 2 - 5); j < int(y_len / 2 + 5); j++) {
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
	void demoCherenkov()
	{
		if (particles.size() == 0) {
			initNewParticle(el_charge, me, 10, 100, 0.7, 0);
		}

		for (int i = int(x_len/8); i < x_len; i++) {
			for (int j = 0; j < y_len; j++) {
				SimReg[t_last][i][j].eps = 12;
				SimReg[t_last][i][j].mu = 1;
			}
		}
		PICdistribution();
		ExplicitTM();
		PICvel();
		PICpos();
		PICdistribution();
		PBC();
	}
public:
	void demoCherenkov2()
	{
		if (particles.size() == 0) {
			//initNewParticle(el_charge, me, 100, 100, 50, 0);
			//initNewParticle(el_charge, me, 80, 100, 50, 0);
			//initNewParticle(el_charge, me, 60, 100, 50, 0);
			//initNewParticle(el_charge, me, 40, 100, 50, 0);
			initNewParticle(el_charge, me, 20, 100, 0.5, 0);
		}
		for (int i = 0; i < x_len; i++) {
			for (int j = 103; j < y_len; j++) {
				SimReg[t_last][i][j].eps = 12;
				SimReg[t_last][i][j].mu = 1;
			}
		}
		/*for (int i = int(x_len / 2)+10; i < x_len; i++)
		{
			for (int j = 0; j < y_len; j++)
			{
				SimReg[t_last][i][j].eps = 11.9;
			}
		}*/
		//sourceOnePoint(1, el_charge * 200, false);
		ExplicitTM();
		PICpos();
		PICvel();
		PBC();
		//ExplicitTEM();
		//ExplicitTE();
		//PEC();
		/*
		for (int i = 0; i < int(x_len / 4); i++) {
			for (int j = 0; j < y_len; j++) {
				SimReg[t_last][i][j].eps = 10;
			}
		}*/
		/*for (int j = 0; j < int(y_len / 4); j++) {
			for (int i = int(y_len / 4); i < int(3*y_len / 4); i++) {
				SimReg[t_last][i][j].eps = 10;
			}
		}
		for (int j = int(3*y_len / 4); j < y_len; j++) {
			for (int i = int(y_len / 4); i < int(3 * y_len / 4); i++) {
				SimReg[t_last][i][j].eps = 10;
			}
		}*/
		
		
		//primitiveABC();
	}
public:
	void demoPIC()
	{
		if (particles.size() == 0) {
			initNewParticle(el_charge, me, 110, 110, 0, 0);
			initNewParticle(el_charge, me, 90, 90, 0, 0);
			initNewParticle(el_charge, me, 110, 90, 0, 0);
			initNewParticle(el_charge, me, 90, 110, 0, 0);
			//initNewParticle(10000000*el_charge, 1000*me, 100, 100, 0, 0);
		}
		sourceOnePoint((x_len / 2), (y_len / 2), 0.5, 7e6*el_charge, true);
		sourceOnePoint((x_len / 2), (y_len / 2), 0.5, 7e6*el_charge, false);
		PICpos();
		ExplicitTEM();
		PICvel();
		PBC();
		//PICdistribution();
	}
};
