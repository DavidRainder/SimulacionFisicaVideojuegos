#include "RenderUtils.hpp"
#pragma once
class Particle
{
public:
	Particle(Vector3 Pos, Vector3 Vel);
	~Particle() {
		if (rI != nullptr) rI->release();
	}

	void integrate(double t);

private:
	Vector3 vel;
	physx::PxTransform pose;
	RenderItem* rI; // render item

};

