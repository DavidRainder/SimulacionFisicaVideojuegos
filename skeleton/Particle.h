#include "RenderUtils.hpp"
#pragma once
class Particle
{
public:
	Particle(Vector3 Pos, Vector3 vel, Vector3 acc, float gravity = 0.0f, float scale = 1, float damping = 0.98f, float maxTimeAlive = 3.0f);
	virtual ~Particle() {
		if (rI != nullptr) rI->release();
		rI = nullptr;
	}
	virtual void integrate(double t);
	inline bool getDestroy() { return destroy; }
protected:
	float g;
	bool destroy = false;
	Vector3 vel, acc;
	float damping;
	physx::PxTransform pose;
	RenderItem* rI; // render item

	float timer = 0;
	float maxTimeAlive;
	inline void Destroy() { destroy = true; }
};

