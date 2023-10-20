#include "RenderUtils.hpp"
#pragma once

struct Particle_config {
	Vector3 vel, acc;
	float damping, maxTimeAlive, g;
	Vector4 color;
	Particle_config(float damping, float maxTimeAlive, float g, 
		Vector4 color = { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }) {
		Particle_config(Vector3(0, 0, 0), Vector3(0, 0, 0), damping, maxTimeAlive, g);
	};
	Particle_config(Vector3 vel, Vector3 acc, float damping, float maxTimeAlive, float g,
		Vector4 color = { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }) : vel(vel),
		acc(vel), damping(damping), maxTimeAlive(maxTimeAlive), g(g), color(color) {}

	Particle_config(Particle_config& p) {
		// construir por copia;
	}
};

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 vel, Vector3 acc, bool usesGravity, float scale = 1, float damping = 0.98f, float maxTimeAlive = 3.0f);
	Particle(Vector3 Pos, Vector3 vel, Vector3 acc, bool usesGravity, Vector4 color, float scale = 1, float damping = 0.98f, float maxTimeAlive = 3.0f);
	virtual ~Particle() {
		if (rI != nullptr) rI->release();
		rI = nullptr;
	}
	virtual void integrate(double t);
	inline bool getDestroy() { return destroy; }
protected:
	float g = -9.8f;
	bool usesGravity;
	bool destroy = false;
	Vector3 vel, acc;
	float damping;
	physx::PxTransform pose;
	RenderItem* rI; // render item

	float timer = 0;
	float maxTimeAlive;
	inline void Destroy() { destroy = true; }
};

