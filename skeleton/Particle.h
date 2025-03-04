#include "RenderUtils.hpp"
#pragma once

template<class T, class Model_Config>
class ParticleGenerator;

enum ParticleType { Sphere, Cube, Plane };

struct Particle_config {
	Vector3 pos, vel;
	float damping, maxTimeAlive, scale, mass;
	bool usesGravity;
	ParticleType _type = Sphere;
	Vector4 color;
	Particle_config(float damping, float maxTimeAlive, float scale, bool usesGravity, float mass, ParticleType _type = Sphere, 
		Vector4 color = { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }) : 
		Particle_config(Vector3(0, 0, 0), Vector3(0, 0, 0), damping, maxTimeAlive, scale, usesGravity, mass, _type, color)  {};
	Particle_config(Vector3 pos, Vector3 vel, float damping, float maxTimeAlive, float scale, bool usesGravity, float mass, ParticleType _type = Sphere,
		Vector4 color = { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }) : 
		pos(pos), vel(vel), damping(damping), maxTimeAlive(maxTimeAlive), color(color), usesGravity(usesGravity), mass(mass), scale(scale), _type(_type) {}

	Particle_config(Particle_config& p) {
		// construir por copia;
		this->mass = p.mass;
		this->pos = p.pos;
		this->vel = p.vel;
		this->damping = p.damping;
		this->maxTimeAlive = p.maxTimeAlive;
		this->usesGravity = p.usesGravity;
		this->color = p.color;
		this->scale = p.scale;
		this->_type = p._type;
	}
};

class Particle
{
public:
	Particle(Vector3 Pos, Vector3 vel, ParticleType type = Sphere, bool usesGravity = false, float mass = 1.0f, float scale = 1, float damping = 0.98f, float maxTimeAlive = 3.0f);
	Particle(Vector3 Pos, Vector3 vel, ParticleType type = Sphere, Vector4 color = { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }, bool usesGravity = false, float mass = 1.0f, float scale = 1, float damping = 0.98f, float maxTimeAlive = 3.0f);
	Particle(physx::PxScene* _scene, const Particle_config& _pC);
	virtual ~Particle() {
		if (rI != nullptr) rI->release();
		rI = nullptr;
	}

	virtual void integrate(double t);
	inline bool getDestroy() { return destroy; }

	inline Vector3 getPos() { return pose.p; }
	inline Vector3 getVel() { return vel; }
	inline float getInvMass() { return 1.0f / mass; }
	inline float getMass() { return mass; }
	inline float getVolume() { return volume; }
	inline float getScale() { return scale; }
	inline float getHeight() { return scale * 2; }

	inline bool generatesOnDeath() { return generatesParticles; }
	inline ParticleGenerator<Particle, Particle_config>* getPG() { return generator; }
	inline void addForce(const Vector3& f) { force += f; }
protected:
	inline void clearAccum() { force *= 0.0f; }; // clear accumulated force
	Vector3 force = {0,0,0}; // accumulated force
	float mass;
	ParticleGenerator<Particle, Particle_config>* generator = nullptr;
	bool generatesParticles = false;
	bool usesGravity;
	bool destroy = false;
	Vector3 vel;
	float damping;
	float volume;
	float scale;
	physx::PxTransform pose;
	RenderItem* rI; // render item

	float timer = 0;
	float maxTimeAlive;
	virtual void Destroy() { destroy = true; }
};