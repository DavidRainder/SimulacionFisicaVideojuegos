#pragma once

#include "ForceGenerator.h"
#include "core.hpp"

template <class T>
class ParticleDragGenerator : public ForceGenerator<T> {
public:
	ParticleDragGenerator(Vector3 windForce, const float k1, const float k2 = 0, 
		BoundingBox bb = BoundingBox(Point(0, 0, 0), Point(0, 0, 0)), float duration = 1e18) 
		: ForceGenerator(bb, duration) {
		setDrag(k1, k2);
		this->windForce = windForce;
	};
	ParticleDragGenerator(Vector3 windForce, const float k1, const float k2 = 0,
		float duration = 1e18) : ForceGenerator(duration) {
		setDrag(k1, k2);
		this->windForce = windForce;
	};
	void updateForce(T* particle, double t) override {
		if (fabs(particle->getInvMass()) < -1e10) return;
		if ((!usesBB) || (_bb.insideBoundingBox(particle->getPos()))) {
			Vector3 v = particle->getVel();
			Vector3 dragF;
			Vector3 newVel = (windForce - v); // (10,0,0) - v = (10 - X, -v.y, -v.z)
			dragF = _k1 * newVel + _k2 * newVel.magnitude() * newVel;
			particle->addForce(dragF);
		}
	}
	inline void setDrag(float k1, float k2) { _k1 = k1; _k2 = k2; };
	inline float getK1() { return _k1; }
	inline float getK2() { return _k2; }

protected:
	Vector3 windForce = { 0,0,0 };
	float _k1;
	float _k2 = 0;
};