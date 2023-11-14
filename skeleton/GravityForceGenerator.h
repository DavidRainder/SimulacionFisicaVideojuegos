#pragma once

#include "ForceGenerator.h"
#include "core.hpp"

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(const Vector3& f) : ForceGenerator() { _gravity = f; };

	virtual void updateForce(Particle* p, double t);

	// inline void setGravity(Vector3 g) { _gravity = g; }

private:
	Vector3 _gravity;
};