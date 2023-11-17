#define _USE_MATH_DEFINES

#include "ExplosionForceGenerator.h"
#include <cmath>

void ExplosionForceGenerator::updateForce(Particle* p, double t) {
	Vector3 pPos = p->getPos();
	if ((!usesBB) || (_bb.insideBoundingBox(pPos))) {
		time += t;
		Vector3 force = Vector3(0,0,0);
		float distDiff = (pPos - pos).magnitude();
		const float timeConst = 20.0f;
		if (time < 4 * timeConst) {
			r = velocity.magnitude() * time;

			force = (explosionForce / (distDiff * distDiff)) * (pPos - pos) * exp(-time / timeConst);
			if (distDiff < r) p->addForce(force);
		}
	}
}
