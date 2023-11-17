#pragma once

#include "ForceGenerator.h"
#include "core.hpp"

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(const Vector3& f, BoundingBox bb = BoundingBox(Point(0, 0, 0), Point(0, 0, 0))) : 
		ForceGenerator(bb) { _gravity = f; };

	virtual void updateForce(Particle* p, double t);

	// inline void setGravity(Vector3 g) { _gravity = g; }

private:
	Vector3 _gravity;
};