#include "GameManager.h"
#include <iostream>

std::vector<int> GameManager::texts_index = std::vector<int>();

GameManager::GameManager(physx::PxScene* scene, physx::PxPhysics* physics, ParticleSystem<Particle, Particle_config>* particleSys,
	ParticleSystem<RigidSolid, RigidSolid_config>* solidSys, int numPlayers)
	: numPlayers(numPlayers), scene(scene), physics(physics),particleSys(particleSys), solidSys(solidSys) {
	currentPlayer = -1;
	texts_index = std::vector<int>(numTexts);
	addTextToRender(Start_Text);
}

void GameManager::addTextToRender(TextType _type) {
	switch (_type) {
	case Start_Text:
		texts_index[Start_Text] = addText(Vector2(WINDOW_WIDTH * 0.3, WINDOW_HEIGHT / 2), texts_strings[Start_Text], Vector4(0, 1, 0, 1));
		break;
	case BuildControls_Text:
		texts_index[BuildControls_Text] = addText(Vector2(0, WINDOW_HEIGHT - 50), texts_strings[BuildControls_Text], Vector4(0, 1, 0, 1));
		break;
	case BuildTime_Text:
		texts_index[BuildTime_Text] = addText(Vector2(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT - 20), texts_strings[BuildTime_Text], Vector4(1, 1, 1, 1));
		break;
	case BuildPieces_Text: 
		texts_index[BuildPieces_Text] = addText(Vector2(20, 20), texts_strings[BuildPieces_Text], Vector4(1, 1, 1, 1));
		break;

	case PlayerID_Text:
		texts_index[PlayerID_Text] = addText(Vector2(WINDOW_WIDTH - 110, 20), texts_strings[PlayerID_Text], Vector4(1, 1, 1, 1));
		break;
	case CannonControls_Text:
		texts_index[CannonControls_Text] = addText(Vector2(0, WINDOW_HEIGHT - 50), texts_strings[CannonControls_Text], Vector4(0, 1, 0, 1));
		break;
	case Winners_Text:
		texts_index[Winners_Text] = addText(Vector2(WINDOW_WIDTH * 0.40, WINDOW_HEIGHT / 2), texts_strings[Winners_Text], Vector4(0, 1, 1, 1));
		break;
	case Restart_Text:
		texts_index[Restart_Text] = addText(Vector2(WINDOW_WIDTH * 0.25, 100), texts_strings[Restart_Text], Vector4(0, 1, 0, 1));
		break;
	}
}

void GameManager::removeTextFromRender(TextType _type) {
	if (texts_index[_type] < 0) return;
	removeText(texts_index[_type]);
	texts_index[_type] = -1;
}

void GameManager::updateTextFromRender(TextType _type, std::string newText) {
	updateText(texts_index[_type], newText);
}

void GameManager::generatePlayers() {
	int seed = rand() % 1000;
	for (int i = 0; i < this->numPlayers; ++i) {
		Player* player = new Player();
		int offset = 25;
		int x = (i / 2) * offset;
		int z = offset;
		if (i % 2 == 0) z = -z;
		Vector3 playerPos = Vector3(x, 3, z);
		player->dropMngr = new DroppingObjectsManager(scene, physics, solidSys, playerPos, timeToBuild, seed);
		player->cannon = new FiringCannon(scene, physics, Models::Solid_Projectiles[0], Vector3(playerPos.x, 4.5f, 0)
			, Vector3(0, 0, playerPos.z).getNormalized(), 6000);
		player->setParticleSystem(particleSys, solidSys);
		_players.push_back(player);
	}
}

void GameManager::StartGame() {
	removeTextFromRender(Start_Text);
	generatePlayers();
	state = Build;

	addTextToRender(BuildControls_Text);
	addTextToRender(BuildTime_Text);
	addTextToRender(BuildPieces_Text);
	addTextToRender(PlayerID_Text);

	NextPlayer();
}

void GameManager::StartCannonPhase() {
	state = Cannon;
	removeTextFromRender(BuildControls_Text);
	removeTextFromRender(BuildTime_Text);
	removeTextFromRender(BuildPieces_Text);
	addTextToRender(CannonControls_Text);

	currentPlayer = -1;
	NextPlayer();
}

void GameManager::StartFinalPhase() {
	state = End;
	removeTextFromRender(PlayerID_Text);
	removeTextFromRender(CannonControls_Text);
	int i = 0;
	vector<int> points_index_player(numPlayers);
	int maxPoints = 0;
	for (auto it : _players) {
		(it)->points = (*it).dropMngr->numObjectsStillStanding();
		points_index_player[i] = (it)->points;
		maxPoints = max(maxPoints, (it)->points);
		++i;
	}
	
	i = 0;
	queue<int> winners;
	for (auto it : points_index_player) {
		if (maxPoints == it) {
			_players[i]->isWinner = true;
			_players[i]->activateFireworks();
			winners.push(i);
		}
		++i;
	}
	
	addTextToRender(Winners_Text);
	std::string winner_text = "MAYOR PUNTUACION: ";
	while (!winners.empty()) {
		winner_text += "\n\t - JUGADOR " + std::to_string(winners.front() + 1);
		winners.pop();
	}
	updateTextFromRender(Winners_Text, winner_text);
	SetCamera(Vector3(0, 45, 0), Vector3(0.1, -1, camLookingDir), true);
}

void GameManager::handleCameraMovement(double t) {
	camLookingDir += cam_dir * (0.05f / abs(camLookingDir + 0.0001f)) * t;
	if (camLookingDir > .5f) cam_dir = -1;
	else if (camLookingDir < -.5f)cam_dir = 1;
	else if (camLookingDir > -0.1f && camLookingDir < 0.1f) camLookingDir = abs(camLookingDir) * cam_dir;
	SetCamera(Vector3(-15, 45, 0), Vector3(0.3, -1, camLookingDir));
}

void GameManager::onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
	_players[currentPlayer]->onCollision(actor1, actor2);
}


void GameManager::NextPlayer() {
	currentPlayer++;
	updateTextFromRender(PlayerID_Text, texts_strings[PlayerID_Text] + std::to_string(currentPlayer + 1));
	switch (state) {
	case Build:
		_players[currentPlayer]->dropMngr->StartGame();
		break;
	case Cannon:
		_players[currentPlayer]->cannon->SetupCannon();
		break;
	}
}


void GameManager::updateOnBuildTexts() {
	auto drop = _players[currentPlayer]->dropMngr;
	float _numTime = drop->getTimeLeftToBuild();
	std::string time = std::to_string(_numTime);
	while ((time[1] == '.' && time.size() > 4) || (time[2] == '.' && time.size() > 5)) {
		time.pop_back();
	}
	updateTextFromRender(BuildTime_Text, time);
	int maxPieces = drop->MaxPieces();
	int numPieces = drop->NumPieces();
	updateTextFromRender(BuildPieces_Text, std::to_string(numPieces) + "/" + std::to_string(maxPieces));
}

void GameManager::update(double t) {
	switch (state) {
	case Menu:
		// do nothing for now
		break;

	case Build:
		if (!_players[currentPlayer]->hasEndedBuildPhase()) {
			updateOnBuildTexts();
			_players[currentPlayer]->updateDrop(t);
		}
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
			if (currentPlayer != numPlayers - 1) {
				NextPlayer();
			}
			else {
				StartFinalPhase();
			}
		}
		break;
	case End:
		handleCameraMovement(t);
		handleRestartText(t);
		break;
	case PressToRestart:
		handleCameraMovement(t);
		break;
	}
}

void GameManager::handleRestartText(double t) {
	if (restarted) return;
	restartTimer += t;
	if (restartTimer > timeTillRestart) {
		restarted = true;
		addTextToRender(Restart_Text);
		state = PressToRestart;
		restartTimer = 0;
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
	case PressToRestart:
		if (key == ' ') {
			destroy = true;
		}
	}

}


GameManager::~GameManager() {
	for (auto it = _players.begin(); it != _players.end(); ) {
		if (*it != nullptr) {
			delete (*it);
			*it = nullptr;
		}
		it = _players.erase(it);
	}

	for (int i = 0; i < texts_index.size(); ++i) {
		removeTextFromRender((GameManager::TextType)i);
	}
}
