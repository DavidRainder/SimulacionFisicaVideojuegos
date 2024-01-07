#pragma once
#include "Projectile.h"
#include <iostream>

Projectile::Projectile(Vector3 pos, Vector3 dir, float scale, Type pType, float scaleSpeed, float maxTimeAlive) : 
	Particle(pos, Vector3(0, 0, 0), Sphere, true, 1.0f, 1.0f, 0.98f, 3.0f){
	switch (pType) {
	case Type::Canonball: { // cannonball (heavy)
		// real ----
		realMass = 20;
		realSpeed = 250;
		// sim ----
		this->speed = 15 * scaleSpeed; // desired speed 

		mass = 5000;
		vel = 15 * dir;
		break;
	}
	case Type::Bullet: { // bullet (pistol ammo)
		// real ----
		realMass = 0.02; // IRL mass
		realSpeed = 330; // IRL speed
		// sim ----
		this->speed = 30 * scaleSpeed; // desired speed

		mass = 2.5f;
		vel = 30 * dir;
		break;
	}
	}
}

void Projectile::integrate(double t) {
	Particle::integrate(t);
}