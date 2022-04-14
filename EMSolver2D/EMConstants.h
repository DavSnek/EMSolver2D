#pragma once
// This file contains useful physical constants (in SI units), and some useful conversions

//----------------------------------VELOCITIES----------------------------------
int c = 299792458; // Speed of light in vacuum [m/s]

//----------------------------------DISTANCES-----------------------------------
float cer = 2.8179403227e-15; // Classical raduis of electron [m]

//------------------------------------MASSES------------------------------------
float me = 9.1093837e-31; // Electron mass [kg]
float mp = 1.67262192e-27; // Proton mass [kg]

//-----------------------------------CHARGES------------------------------------
float e = 1.60217663e-19; // Electron charge [C]

//-------------------------------ELECTROMAGNETISM--------------------------------
float mu_0  = 1.2566370612e-6; // Vacuum permeability [kg * m * s^-2 * A^-2]
float eps_0 = 8.8541878128e-12;// Vacuum permittivity [s^4 * A^2 * kg^-1 * m^-3]

float getVelocity(float beta) 
{
	// Converges relativistic beta parameter to velocity in [m/s]
	return beta * c;
}

float eVtoVelocity(float e, float mass)
{
	// TODO:
	// Make conversion from kinetic energy in electronVolts to particle velocity in  m/s.
	return 0;
}







