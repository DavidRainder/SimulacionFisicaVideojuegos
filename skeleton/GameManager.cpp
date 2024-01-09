#include "GameManager.h"

GameManager::GameManager(physx::PxScene* scene, physx::PxPhysics* physics, int numPlayers) 
	: numPlayers(numPlayers), scene(scene), physics(physics) {
	for (int i = 0; i < this->numPlayers; ++i) {
		Player* player = new Player();
		int offset = 25;
		int x = (i / 2) * offset;
		int z = offset;
		if (i % 2 == 0) z = -z;
		Vector3 playerPos = Vector3(x, 3, z);
		player->dropMngr = new DroppingObjectsManager(scene, physics, playerPos, 5, 15);
		_players.push_back(player);
	}
	
	currentPlayer = -1;
}

void GameManager::StartGame() {
	state = Build;
	NextPlayer();
}

void GameManager::StartCannonPhase() {
	state = Cannon;
	currentPlayer = 0;
}

void GameManager::NextPlayer() {
	currentPlayer++;
	_players[currentPlayer]->StartGame();
}

void GameManager::update(double t) {
	switch (state) {
	case Menu:
		// do nothing for now
		break;

	case Build:
		if (!_players[currentPlayer]->dropMngr->hasEndedBuildPhase())
			_players[currentPlayer]->update(t);
		else if (currentPlayer != numPlayers - 1) {
			NextPlayer();
		}
		else {
			StartCannonPhase();
		}
		break;
	case Cannon:

		break;
	}
}

void GameManager::keyPressed(unsigned char key, const physx::PxTransform& camera) {
	switch (state) {
	case Menu:
		if (key == ' ') StartGame();
		break;

	case Build:
		_players[currentPlayer]->keyPressed(key, camera);
		break;
	case Cannon:
		_players[currentPlayer]->keyPressed(key, camera);
		break;
	}

}
