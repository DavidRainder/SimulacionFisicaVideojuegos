#include "ParticleDragGenerator.h"

void ParticleDragGenerator::updateForce(Particle* p, double t) {
	if (fabs(p->getInvMass()) < -1e10) return;
	if ((!usesBB) || (_bb.insideBoundingBox(p->getPos()))) {
		Vector3 v = p->getVel();
		Vector3 dragF;
		Vector3 newVel = (windForce - v); // (10,0,0) - v = (10 - X, -v.y, -v.z)
		dragF = _k1 * newVel + _k2 * newVel.magnitude() * newVel;
		p->addForce(dragF);
	}
}