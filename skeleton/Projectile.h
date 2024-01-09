#pragma once
#include "core.hpp"
#include "RigidSolid.h"

#pragma once
class Projectile : public RigidSolid
{
public:
	enum class Type { Canonball, Bullet };
	Projectile(physx::PxScene* scene, physx::PxPhysics* physics, 
		Vector3 pos, Vector3 force, RigidSolid_config& model);
	virtual void integrate(double t);
protected:
	
	const float realGrav = -9.8f;
	float speed, realMass, realSpeed;
};

