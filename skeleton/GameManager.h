#pragma once
#include "Cannon.h"
#include "DroppingObjectsManager.h"

class GameManager
{
private:
	enum State { Menu, Build, Cannon, End };

	State state = Menu;

	struct Player {
		DroppingObjectsManager* dropMngr = nullptr;
		FiringCannon* cannon = nullptr;

		void updateDrop(double t) {
			dropMngr->update(t);
		}

		void updateCannon(double t) {
			cannon->update(t);
		}

		inline bool hasEndedBuildPhase() {
			return dropMngr->hasEndedBuildPhase();
		}

		inline bool hasEndedCannonPhase() {
			return cannon->hasEndedCannonPhase();
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
	void StartFinalPhase();
	void StartCannonPhase();
	void NextPlayer();
};

