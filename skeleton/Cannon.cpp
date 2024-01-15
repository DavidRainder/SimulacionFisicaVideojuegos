#include "Cannon.h"

FiringCannon::FiringCannon(physx::PxScene* scene, physx::PxPhysics* physics, RigidSolid_config* Model, Vector3 pos, Vector3 dir, float force) :
	scene(scene), physics(physics), position(pos), model(Model), force(force), direction(dir) {
}

void FiringCannon::keyPressed(unsigned char key, const physx::PxTransform& camera) {
	switch (key) {
	case ' ':
		shoot();
		break;
	}
}

FiringCannon::~FiringCannon() {
	for (auto it = firedProjectiles.begin(); it != firedProjectiles.end();) {
		if (*it != nullptr)
		{
			delete (*it);
			(*it) = nullptr;
		}
		it = firedProjectiles.erase(it);
	}
}

bool FiringCannon::update(double t) {
	for (auto it = firedProjectiles.begin(); it != firedProjectiles.end();) {
		if (!(*it)->getDestroy()) {
			(*it)->integrate(t);
			++it;
		}
		else {
			delete (*it);
			(*it) = nullptr;

			it = firedProjectiles.erase(it);
		}
	}

	if (timerActivated) {
		timer += t;
		if (timer >= maxTime) {
			timerActivated = false;
			ended = true;
		}
	}
	return timerActivated;
}

void FiringCannon::startCounter() {
	timer = 0;
	timerActivated = true;
}

void FiringCannon::shoot() {
	if (numShots >= maxShots) return;
	Vector3 dir = GetCamera()->getDir().getNormalized();
	Vector3 pos = GetCamera()->getEye();
	RigidSolid* cannonBall = new RigidSolid(scene, physics, *model);
	cannonBall->setPos(pos);
	cannonBall->addForce(force * dir);

	firedProjectiles.push_back(cannonBall);
	numShots++;
	if (numShots >= maxShots) {
		startCounter();
	}
}