#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"
#include <iostream>

Particle::Particle(Vector3 pos, Vector3 vel, ParticleType type, bool usesGravity, float mass, float scale, float damping, float maxTimeAlive) : Particle(pos, vel, type, { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }, usesGravity, mass,
		scale, damping, maxTimeAlive) {}

Particle::Particle(Vector3 pos, Vector3 vel, ParticleType type, Vector4 color, bool usesGravity, float mass, float scale, float damping, float maxTimeAlive) {
	pose = physx::PxTransform(pos); // posición
	physx::PxShape* shape;
	if (type == Sphere) {
		physx::PxSphereGeometry sphere(scale); // escala
		shape = CreateShape(sphere);
	}
	else if (type == Cube) {
		physx::PxBoxGeometry box(Vector3(1,1,1) * scale);
		shape = CreateShape(box);
	}
	else if (type == Plane) {
		physx::PxBoxGeometry box(Vector3(500, 5, 500) * scale);
		shape = CreateShape(box);
	}
	this->mass = mass; // masa
	this->vel = vel; // velocidad
	this->usesGravity = usesGravity; // usa gravedad?
	this->damping = damping; // damping
	this->maxTimeAlive = maxTimeAlive; // máximo tiempo de vida
	this->scale = scale;
	this->volume = pow(scale, 3);

	rI = new RenderItem(shape, &pose, color); // color
}

Particle::Particle(physx::PxScene* _scene, const Particle_config& _pC) : Particle(_pC.pos, _pC.vel, _pC._type, _pC.color, _pC.usesGravity, _pC.mass, _pC.scale, _pC.damping, _pC.maxTimeAlive){}

void Particle::integrate(double t) {
	// Get the accel considering the force accum
	Vector3 resulting_accel = force * getInvMass();
	vel += resulting_accel * t; // Ex. 1.3 --> add acceleration
	vel *= powf(damping, t); // Exercise 1.3 --> add damping
	timer += t;
	pose.p += vel * t; 
	// Clear accum
	clearAccum();
	if (maxTimeAlive > 0 && timer > maxTimeAlive) {
		Destroy();
	}
}