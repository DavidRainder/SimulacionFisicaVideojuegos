#include "Projectile.h"

#pragma once
class FiringCannon
{
private:
	Vector3 position, direction; 
	std::vector<RigidSolid*> firedProjectiles;

	physx::PxScene* scene;
	physx::PxPhysics* physics;

	const int maxShots = 1;
	int numShots = 0;

	RigidSolid_config* model;
	float force;

	bool ended = false;

	bool timerActivated = false;
	float maxTime = 5;
	float timer = 0;

public:
	FiringCannon(physx::PxScene* scene, physx::PxPhysics* physics, RigidSolid_config* Model, Vector3 pos, Vector3 dir, float force);

	inline void SetupCannon() {
		// dir is just for adjusting the camera
		SetCamera(position, direction);
	}

	void keyPressed(unsigned char key, const physx::PxTransform& camera);

	void update(double t);

	inline bool hasEndedCannonPhase() {
		return ended;
	}

private:
	void startCounter();
	void shoot();
};

