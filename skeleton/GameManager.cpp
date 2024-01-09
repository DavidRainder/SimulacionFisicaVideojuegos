#include "GameManager.h"
#include <iostream>

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
		player->cannon = new FiringCannon(scene, physics, Models::Solid_Projectiles[0], Vector3(playerPos.x, 3.f, 0)
			, Vector3(0, 0, playerPos.z).getNormalized(), 3000);
		
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
	currentPlayer = -1;
	NextPlayer();
}

void GameManager::NextPlayer() {
	currentPlayer++;
	switch (state) {
	case Build:
		_players[currentPlayer]->dropMngr->StartGame();
		break;
	case Cannon:
		_players[currentPlayer]->cannon->SetupCannon();
		break;
	}
}

void GameManager::StartFinalPhase() {
	state = End;
	for (auto it : _players) {
		(it)->points = (*it).dropMngr->numObjectsStillStanding();
	}

	SetCamera(Vector3(0, 70, 0), Vector3(0.1, -1, 0.1), true); 
}
void GameManager::update(double t) {
	switch (state) {
	case Menu:
		// do nothing for now
		break;

	case Build:
		if (!_players[currentPlayer]->hasEndedBuildPhase())
			_players[currentPlayer]->updateDrop(t);
		else if (currentPlayer != numPlayers - 1) {
			NextPlayer();
		}
		else {
			StartCannonPhase();
		}
		break;
	case Cannon:
		if (!_players[currentPlayer]->hasEndedCannonPhase())
			_players[currentPlayer]->updateCannon(t);
		else {
			_players[currentPlayer]->dropMngr->switchToStaticPieces();
			if (currentPlayer != numPlayers - 1) {
				NextPlayer();
			}
			else {
				StartFinalPhase();
			}
		}
		break;
	case End:

		break;
	}
}

void GameManager::keyPressed(unsigned char key, const physx::PxTransform& camera) {
	switch (state) {
	case Menu:
		if (key == ' ') StartGame();
		break;

	case Build:
		_players[currentPlayer]->dropMngr->keyPressed(key, camera);
		break;
	case Cannon:
		_players[currentPlayer]->cannon->keyPressed(key, camera);
		break;
	}

}
