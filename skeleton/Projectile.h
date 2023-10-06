#include "Particle.h"

#pragma once
class Projectile : public Particle
{
public:
	enum class Type { Canonball, Bullet };
	Projectile(Vector3 pos, Vector3 dir, float scale, Type pType, float speed = 1.0f, float maxTimeAlive = 3.0f);
	virtual void integrate(double t);
protected:
	void calculateMass() {
		mass = realMass * pow(realSpeed, 2) / speed;
	}
	void calculateGravity() {
		// grav = realGrav * pow((speed / realSpeed), 2); La fórmula real es esta: (vSim/vReal)^2, pero si aplicamos esto la bala 
		// tarda mucho tiempo en caer. He modificado la fórmula quitando el ^2 para que genere valores que se acerquen más al
		// resultado que queremos simular
		g = realGrav * (speed / realSpeed);
		acc += Vector3(0,1,0) * g;
	}
	
	const float realGrav = -9.8f;
	float mass, speed, realMass, realSpeed;
};

