#pragma once

#include "ForceGenerator.h"
#include "core.hpp"

class ParticleDragGenerator : public ForceGenerator {
public:
	ParticleDragGenerator(Vector3 windForce, const float k1, const float k2 = 0, 
		BoundingBox bb = BoundingBox(Point(0, 0, 0), Point(0, 0, 0))) : ForceGenerator(bb) {
		setDrag(k1, k2);
		this->windForce = windForce;
	};
	virtual void updateForce(Particle* particle, double t);
	inline void setDrag(float k1, float k2) { _k1 = k1; _k2 = k2; };
	inline float getK1() { return _k1; }
	inline float getK2() { return _k2; }

protected:
	Vector3 windForce = { 0,0,0 };
	float _k1;
	float _k2 = 0;
};