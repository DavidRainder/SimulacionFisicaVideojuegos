#include "Particle.h"
#include "ParticleGenerator.h"
#include <iostream>

Particle::Particle(Vector3 pos, Vector3 vel, bool usesGravity, float mass, float scale, float damping, float maxTimeAlive) : Particle(pos, vel, { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }, usesGravity, mass,
		scale, damping, maxTimeAlive) {}

Particle::Particle(Vector3 pos, Vector3 vel, Vector4 color, bool usesGravity, float mass, float scale, float damping, float maxTimeAlive) {
	pose = physx::PxTransform(pos); // posición
	physx::PxSphereGeometry sphere(scale); // escala
	physx::PxShape* shape = CreateShape(sphere);
	this->mass = mass; // masa
	this->vel = vel; // velocidad
	this->usesGravity = usesGravity; // usa gravedad?
	this->damping = damping; // damping
	this->maxTimeAlive = maxTimeAlive; // máximo tiempo de vida
	rI = new RenderItem(shape, &pose, color); // color
}

Particle::Particle(const Particle_config& _pC) : Particle(_pC.pos, _pC.vel, _pC.color, _pC.usesGravity, _pC.mass, _pC.scale, _pC.damping, _pC.maxTimeAlive){}

void Particle::integrate(double t) {
	// Get the accel considering the force accum
	Vector3 resulting_accel = force * getInvMass();
	std::cout << getInvMass() << '\n';
	vel += resulting_accel * t; // Ex. 1.3 --> add acceleration
	vel *= powf(damping, t); // Exercise 1.3 --> add damping
	pose.p += vel * t; // i don't think its necessary to multiply again by delta_t. 
	timer += t;
	// Clear accum
	clearAccum();
	if (timer > maxTimeAlive) {
		Destroy();
	}
}

Firework::Firework(int generation, Vector3 Pos, Vector3 vel, bool usesGravity, float mass, float scale, float damping, float maxTimeAlive)
	: Particle(Pos, vel, { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }, usesGravity, mass,
		scale, damping, maxTimeAlive), generation(generation) {
	if (generation != 0) {
		generatesParticles = true;
		this->_pC = new Particle_config(Pos, vel, damping, maxTimeAlive * 0.7f, scale, usesGravity, mass);
		generator = new FireworkExplosionGenerator(generation - 1, "Firework " + std::to_string(Pos.x), { 50,50,50 }, Pos, 10);
		generator->setParticleModel(this->_pC);
	}
};

Firework::Firework(int generation, Vector3 Pos, Vector3 vel, Vector4 color, bool usesGravity, float mass, float scale, float damping, float maxTimeAlive)
	: Particle(Pos, vel, { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }, usesGravity, mass,
		scale, damping, maxTimeAlive), generation(generation) {
	if (generation != 0) {
		generatesParticles = true;
		this->_pC = new Particle_config(Pos, vel, damping, maxTimeAlive * 0.7f, scale, usesGravity, mass, color);
		generator = new FireworkExplosionGenerator(generation - 1, "Firework " + std::to_string(Pos.x), { 50,50,50 }, Pos, 10);
		Particle_config newPC = Particle_config((*this->_pC));
		generator->setParticleModel(_pC);
	}
};

Firework::Firework(int generation, Particle_config& _pC) : 
	Particle(_pC.pos, _pC.vel, _pC.color, _pC.usesGravity, _pC.mass, _pC.scale, _pC.damping, _pC.maxTimeAlive * 0.7f), generation(generation) {
	if (generation != 0) {
		generatesParticles = true;
		this->_pC = new Particle_config(_pC);
		generator = new FireworkExplosionGenerator(generation - 1, "Firework " + std::to_string(_pC.pos.x), { 50,50,50 }, _pC.pos, 10);
		generator->setParticleModel(this->_pC);
	}
};

void Firework::Destroy() {
	Particle::Destroy();
	if(generation != 0) generator->changePos(pose.p);
}

void Firework::integrate(double t) {
	Particle::integrate(t);
	rI->color.w = 0;
}