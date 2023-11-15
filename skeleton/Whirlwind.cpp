#include "Whirlwind.h"

void WhirlwindForceGenerator::updateForce(Particle* p, double t) {
	if (fabs(p->getInvMass()) < -1e10) return;

	Vector3 pPos = p->getPos();
	Vector3 force = windForce * Vector3(-(p->getPos().z - centerPos.z),
		(p->getPos().y - centerPos.y), p->getPos().x - centerPos.x);

	p->addForce(force);
}
