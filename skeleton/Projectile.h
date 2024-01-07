#pragma once
#include "Particle.h"

#pragma once
class Projectile : public Particle
{
public:
	enum class Type { Canonball, Bullet };
	Projectile(Vector3 pos, Vector3 dir, float scale, Type pType, float speed = 1.0f, float maxTimeAlive = 3.0f);
	virtual void integrate(double t);
protected:
	
	const float realGrav = -9.8f;
	float speed, realMass, realSpeed;
};

