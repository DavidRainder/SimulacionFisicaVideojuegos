#pragma once
#include "Cannon.h"
#include "DroppingObjectsManager.h"

class GameManager
{
private:
	enum State { Menu, Build, Cannon };

	State state = Menu;

	struct Player {
		DroppingObjectsManager* dropMngr = nullptr;
		FiringCannon* cannon = nullptr;

		void update(double t) {
			dropMngr->update(t);
		}

		void keyPressed(unsigned char key, const physx::PxTransform& camera) {
			dropMngr->keyPressed(key, camera);
		}

		void StartGame() {
			dropMngr->StartGame();
		}
	};

	std::vector<Player*> _players;

	int numPlayers = 2;
	int currentPlayer;

	physx::PxScene* scene = nullptr;
	physx::PxPhysics* physics = nullptr;

public:
	GameManager(physx::PxScene* scene, physx::PxPhysics* physics, int numPlayers);

	void StartGame();

	void update(double t);

	void keyPressed(unsigned char key, const physx::PxTransform& camera);

private:
	void StartCannonPhase();
	void NextPlayer();
};

