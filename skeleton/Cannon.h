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
	float maxTime = 3.5f;
	float timer = 0;

public:
	FiringCannon(physx::PxScene* scene, physx::PxPhysics* physics, RigidSolid_config* Model, Vector3 pos, Vector3 dir, float force);
	~FiringCannon();
	inline void SetupCannon() {
		// dir is just for adjusting the camera
		SetCamera(position, direction, true);
	}

	void keyPressed(unsigned char key, const physx::PxTransform& camera);

	void update(double t);

	inline bool hasEndedCannonPhase() {
		return ended;
	}

	inline std::vector<RigidSolid*> FiredProjectiles() { return firedProjectiles; }

private:
	void startCounter();
	void shoot();
};

