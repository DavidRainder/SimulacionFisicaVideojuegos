#pragma once
#include "ForceGenerator.h"

template <class T>
class ExplosionForceGenerator : public ForceGenerator<T> {
public:
	ExplosionForceGenerator(Vector3 pos, float explosionForce, float r, 
		BoundingBox bb, float duration = 1e18) : 
		ForceGenerator(bb), pos(pos), explosionForce(explosionForce), r(r), velocity(r) {}

	ExplosionForceGenerator(Vector3 pos, float explosionForce, float r, float duration = 1e18) :
		ForceGenerator(duration), pos(pos), explosionForce(explosionForce), r(r), velocity(r) {}

	void updateForce(T* p, double t) override {
		Vector3 pPos = p->getPos();
		if ((!usesBB) || (_bb.insideBoundingBox(pPos))) {
			time += t;
			Vector3 force = Vector3(0, 0, 0);
			float distDiff = (pPos - pos).magnitude();
			const float timeConst = 20.0f;
			if (time < 4 * timeConst) {
				r = velocity.magnitude() * time;

				force = (explosionForce / (distDiff * distDiff)) * (pPos - pos) * exp(-time / timeConst);
				if (distDiff < r) p->addForce(force);
			}
		}
	}

private:
	Vector3 pos, velocity;
	float explosionForce;
	float r;
	float time = 0.0f;
};