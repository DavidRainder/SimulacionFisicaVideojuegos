#pragma once

#include "DroppingObjectsManager.h"

class GameManager
{
private:
	struct Player {
		DroppingObjectsManager* dropMngr = nullptr;
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

};

