#include "Projectile.h"

#pragma once
class FiringCannon
{
private:
	Vector3 position; 
	std::vector<RigidSolid*> firedProjectiles;

public:
	FiringCannon(Vector3 pos);

	void keyPressed(unsigned char key, physx::PxTransform* camera);

	void update();

private:
	void shoot();
};

