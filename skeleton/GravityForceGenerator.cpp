#include "GravityForceGenerator.h"

void GravityForceGenerator::updateForce(Particle* p, double t) {
	if (fabs(p->getInvMass()) < 1e-10)
		return;

	if ((!usesBB) || (_bb.insideBoundingBox(p->getPos()))) 
		p->addForce(_gravity * p->getMass());
}