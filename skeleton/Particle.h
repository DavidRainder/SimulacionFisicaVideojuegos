#include "RenderUtils.hpp"
#pragma once
class Particle
{
public:
	const Vector3 g = Vector3(0,-9.8f,0);
	Particle(Vector3 Pos, Vector3 vel, Vector3 acc, float scale = 1, float damping = 0.98f);
	virtual ~Particle() {
		if (rI != nullptr) rI->release();
	}
	virtual void integrate(double t);
	inline bool getDestroy() { return destroy; }
protected:
	bool destroy = false;
	Vector3 vel, acc;
	float damping;
	physx::PxTransform pose;
	RenderItem* rI; // render item

	inline void Destroy() { destroy = true; }
};

