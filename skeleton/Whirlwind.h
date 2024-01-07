#pragma once
#include "ParticleDragGenerator.h"

template<class T>
class WhirlwindForceGenerator : public ForceGenerator<T> {
public:
	WhirlwindForceGenerator(Vector3 centerPos, const float k
		, BoundingBox bb, float duration = 1e18) :
		ForceGenerator(bb, duration), k(k), centerPos(centerPos) {};
	WhirlwindForceGenerator(Vector3 centerPos, const float k, float duration = 1e18) :
		ForceGenerator(duration), k(k), centerPos(centerPos) {};

	void updateForce(T* p, double t) {
		if (fabs(p->getInvMass()) < -1e10) return;
		Vector3 pPos = p->getPos();
		if ((!usesBB) || (_bb.insideBoundingBox(pPos))) {
			// si no usa BB, entra. 
			// Si la usa y está dentro de los límites, 
			// también entra, pero usándola fuera de los límites no funciona
			Vector3 force = k * Vector3(-(pPos.z - centerPos.z),
				75 - (pPos.y - centerPos.y), pPos.x - centerPos.x);

			force = (force - p->getVel());

			p->addForce(force);
		}
	}
private:
	float k;
	Vector3 centerPos;
};
