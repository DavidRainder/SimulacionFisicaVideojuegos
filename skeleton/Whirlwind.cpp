#include "Whirlwind.h" 

void WhirlwindForceGenerator::updateForce(Particle* p, double t) {
	if (fabs(p->getInvMass()) < -1e10) return;
	Vector3 pPos = p->getPos();
	if ((!usesBB) || (_bb.insideBoundingBox(pPos))) { 
		// si no usa BB, entra. 
		// Si la usa y est� dentro de los l�mites, 
		// tambi�n entra, pero us�ndola fuera de los l�mites no funciona
		Vector3 force = k * Vector3(-(pPos.z - centerPos.z),
			75 - (pPos.y - centerPos.y), pPos.x - centerPos.x);

		force = (force - p->getVel());

		p->addForce(force);
	}
}
