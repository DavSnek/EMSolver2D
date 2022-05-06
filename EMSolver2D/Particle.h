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
	  //TODO:
	  // 1. Function that returns Electric field at a distance from the particle. DONE
	  // 2. Function that calculates magnetic field of moving charge
public: 
	float* getE(float x, float y)
	{
		// Calculates and returns Electric field at position [x,y].
		float E[2];
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
		return E;
	}

};