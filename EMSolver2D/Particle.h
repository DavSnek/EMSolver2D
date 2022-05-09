#pragma once
#include "EMConstants.h"


class Particle 
{
public: float pos[2]; //2d vector of position
public: float v[2]; //2d vector of velocity
public: float charge; // Electric charge
public: float mass; // Mass of the particle

public:
	Particle(float q, float m, float posx, float posy, float vx, float vy) : charge(q), mass(m)
		{
			this->pos[0] = posx;
			this->pos[1] = posy;
			this->v[0] = vx;
			this->v[1] = vy;
		}
public: 
	float* getE(float x, float y)
	{
		// Calculates and returns Electric field at position [x,y].
		float E[3]; // E = {Ex,Ey,Ez}
		float dx = x - pos[0];
		float dy = y - pos[1];
		//float r = sqrt(dx*dx + dy*dy)
		if (abs(dx) >= cer) // if distance is greater than classical electron radius
			E[0] = k * charge / (dx * dx + dy * dy) * dx / abs(dx);
		else
			E[0] = 0;
		if (abs(dy) >= cer)
			E[1] = k * charge / (dy * dy + dx *dx) * dy / abs(dy);
		else
			E[1] = 0;
		if (sqrt(dx*dx + dy*dy) >= cer)
			E[2] = k * charge / (dy * dy + dx * dx);
		else
			E[2] = 0;
		return E;
	}
public:
	float getH(float x, float y, float mu_r)
	{
		// Calculates and returns Magnetic field of moving charge at position [x,y].
		// In 2D only Hz exists (Vz = 0 & dz = 0).
		float H;
		float dx = x - pos[0];
		float dy = y - pos[1];
		H = charge / (4 * PI * mu_r) * (v[0] * dy - v[1] * dx) / (dx * dx + dy * dy);
		return H;
	}

};