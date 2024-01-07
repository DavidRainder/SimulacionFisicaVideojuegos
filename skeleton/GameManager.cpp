#include "GameManager.h"

GameManager::GameManager(physx::PxScene* scene, physx::PxPhysics* physics, int numPlayers) 
	: numPlayers(numPlayers), scene(scene), physics(physics) {
	for (int i = 0; i < this->numPlayers; ++i) {
		Player* player = new Player();
		int offset = 25;
		int x = (i / 2) * offset;
		int z = offset;
		if (i % 2 == 0) z = -z;
		Vector3 playerPos = Vector3(x, 0, z);
		player->dropMngr = new DroppingObjectsManager(scene, physics, playerPos);
	}
}