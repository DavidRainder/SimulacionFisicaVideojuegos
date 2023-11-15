#define _USE_MATH_DEFINES

#include "ExplosionForceGenerator.h"
#include <cmath>

void ExplosionForceGenerator::updateForce(Particle* p, double t) {
	Vector3 force;
	Vector3 pos = p->getPos();
	float distDiff = (pos - this->pos).magnitude();
	if (distDiff < r) {
		(explosionForce / pow(r, 2))* Vector3(pos.x - this->pos.x,
			pos.y - this->pos.y,
			pos.z - this->pos.z)* pow(M_E, (-t/3));
	}
	else force = Vector3(0, 0, 0);

	p->addForce(force);
}
