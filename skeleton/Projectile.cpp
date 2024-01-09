#pragma once
#include "Projectile.h"
#include <iostream>

Projectile::Projectile(physx::PxScene* scene, physx::PxPhysics* physics,
	Vector3 pos, Vector3 force, RigidSolid_config& model) :
	RigidSolid(scene, physics, model){
	setPos(pos);
	addForce(force);
}

void Projectile::integrate(double t) {
	RigidSolid::integrate(t);
}