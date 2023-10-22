#include "RenderUtils.hpp"
#pragma once

class ParticleGenerator;


struct Particle_config {
	Vector3 pos, vel, acc;
	float damping, maxTimeAlive, g, scale;
	bool usesGravity;
	Vector4 color;
	Particle_config(float damping, float maxTimeAlive, float scale, bool usesGravity, float g, 
		Vector4 color = { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }) : Particle_config(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), damping, maxTimeAlive, scale, usesGravity, g, color)  {};
	Particle_config(Vector3 pos, Vector3 vel, Vector3 acc, float damping, float maxTimeAlive, float scale, bool usesGravity, float g,
		Vector4 color = { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }) : pos(pos), vel(vel),
		acc(vel), damping(damping), maxTimeAlive(maxTimeAlive), g(g), color(color), usesGravity(usesGravity), scale(scale) {}

	Particle_config(Particle_config& p) {
		// construir por copia;
		this->pos = p.pos;
		this->vel = p.vel;
		this->acc = p.acc;
		this->damping = p.damping;
		this->maxTimeAlive = p.maxTimeAlive;
		this->g = p.g;
		this->usesGravity = p.usesGravity;
		this->color = p.color;
		this->scale = p.scale;
	}
};

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 vel, Vector3 acc, bool usesGravity = false, float gravity = -9.8f, float scale = 1, float damping = 0.98f, float maxTimeAlive = 3.0f);
	Particle(Vector3 Pos, Vector3 vel, Vector3 acc, Vector4 color = { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }, bool usesGravity = false, float gravity = -9.8f, float scale = 1, float damping = 0.98f, float maxTimeAlive = 3.0f);
	Particle(const Particle_config& _pC);
	virtual ~Particle() {
		if (rI != nullptr) rI->release();
		rI = nullptr;
	}

	virtual void integrate(double t);
	inline bool getDestroy() { return destroy; }

	inline bool generatesOnDeath() { return generatesParticles; }
	inline ParticleGenerator* getPG() { return generator; }
protected:
	ParticleGenerator* generator = nullptr;
	bool generatesParticles = false;
	float g = -9.8f;
	bool usesGravity;
	bool destroy = false;
	Vector3 vel, acc;
	float damping;
	physx::PxTransform pose;
	RenderItem* rI; // render item

	float timer = 0;
	float maxTimeAlive;
	virtual void Destroy() { destroy = true; }
};


class Firework : public Particle {
public:
	Firework(Vector3 Pos, Vector3 vel, Vector3 acc, bool usesGravity = false, float gravity = -9.8f, float scale = 1, float damping = 0.98f, float maxTimeAlive = 3.0f);
	Firework(Vector3 Pos, Vector3 vel, Vector3 acc, Vector4 color = { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }, bool usesGravity = false, float gravity = -9.8f, float scale = 1, float damping = 0.98f, float maxTimeAlive = 3.0f);
	Firework(Particle_config& _pC);

protected:
	Particle_config* _pC;

	virtual void Destroy();
};
