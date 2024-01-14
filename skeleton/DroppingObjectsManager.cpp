#pragma once
#include "DroppingObjectsManager.h"
#include <iostream>

DroppingObjectsManager::DroppingObjectsManager(physx::PxScene* scene, physx::PxPhysics* physics, ParticleSystem<RigidSolid, RigidSolid_config>* particleSys
	, Vector3 pos, float time, int seed) : particleSys(particleSys),
	position(pos), objectPosition(pos), moveDir(Vector3(0, 0, 0)), gScene(scene), gPhysics(physics), timeToBuild(time),
	_bb(Point(-5 + pos.x,1.5f,-5 + pos.z), Point(5 + pos.x,10000000,5 + pos.z)) {

	avaliablePieces = std::vector<int>(maxPieces);
	srand(seed);

	if (pos.z < 0) camOffset = 0;
	else camOffset = 4;
	for (int i = camOffset; i < 4 + camOffset; ++i) {
		cameraPositions[i] += pos;
	}

	// initialize with a given seed
	for (int i = 0; i < maxPieces; ++i) {
		avaliablePieces[i] = rand() % Models::static_dropping_solids.size();
	}

	platform = new RigidSolid(gScene, gPhysics, *Models::Platform[1]);
	float posOffset = 1.0f;
	platform->setPos(Vector3(pos.x, platform->getHeight()/2 + posOffset, pos.z));

	position.y += posOffset;
	
	canBuild = false;
	stopTimer();
}

DroppingObjectsManager::~DroppingObjectsManager() {

}


void DroppingObjectsManager::StartGame() {
	canBuild = true;
	SetCamera(cameraPositions[camOffset], cameraDirections[camOffset], true);
	currentCameraPosition = 0;
	generateRandomObject(position);
	restartTimer();
}

int DroppingObjectsManager::numObjectsStillStanding() {
	int num = 0;
	for (auto it : dynamic_pieces) {
		if(_bb.insideBoundingBox((*it).getPos()))
			num++;
	}
	return num;
}

void DroppingObjectsManager::handleRotation(int dir) {
	if (currentObject == nullptr) return;
	if (dir > 0) dir = 1;
	else dir = -1;
	currentObject->rotate(dir*90, rotation_axis[currentRotation]);
}

void DroppingObjectsManager::keyPressed(unsigned char key, physx::PxTransform Camera) {
	switch (key) {
	case 'w':
		moveDir += moveDirections[currentCameraPosition%2 + camOffset];
		break;
	case 's':
		moveDir += moveDirections[(currentCameraPosition + 1) % 2 + camOffset];
		break;
	case 'a':
		moveDir += moveDirections[(currentCameraPosition) % 2 + 2 + camOffset];
		break;
	case 'd':
		moveDir += moveDirections[(currentCameraPosition + 1) % 2 + 2 + camOffset];
		break;
	case 'z':
		moveDir.y += cameraUpMovement;
		break;
	case 'x':
		moveDir.y -= cameraUpMovement;
		break;
	case 'q':
		handleRotation(1);
		break;
	case 'e': 
		handleRotation(-1);
		break;
	case 'r':
		currentRotation = (currentRotation + 1) % 3;
		break;
	case ' ':
		drop();
		break;
	case '1':
		currentCameraPosition = 0;
		SetCamera(cameraPositions[0 + camOffset], cameraDirections[0 + camOffset]);
		break;
	case '2':
		currentCameraPosition = 3;
		SetCamera(cameraPositions[1 + camOffset], cameraDirections[1 + camOffset]);
		break;
	case '3':
		currentCameraPosition = 1;
		SetCamera(cameraPositions[2 + camOffset], cameraDirections[2 + camOffset]);
		break;
	case '4':
		currentCameraPosition = 2;
		SetCamera(cameraPositions[3 + camOffset], cameraDirections[3 + camOffset]);
		break;
	case 'l':
		switchToDynamicPieces();
		break;
	case 'b':
		undoDrop();
		break;
	default:
		break;
	}
}

void DroppingObjectsManager::undoDrop() {
	if (static_pieces.size() > 0) {
		delete currentObject;
		auto _piece = static_pieces.back();
		currentObject = _piece->solid;
		modelNum = _piece->modelNum;
		static_pieces.erase(--static_pieces.end());
		currentPieces--;
	}
}

void DroppingObjectsManager::generateRandomObject(Vector3 pos) {
	if (currentObject != nullptr) return;
	
	auto it = avaliablePieces.begin() + (rand() % avaliablePieces.size());
	modelNum = *it;
	
	currentObject = new RigidSolid(gScene, gPhysics, *Models::static_dropping_solids[modelNum]);
	avaliablePieces.erase(it);

	_lastWidth = currentObject->getWidth() * 1.02f;
	objectPosition = pos;
	
	cameraUpMovement = currentObject->getHeight() * 1.02f;
	currentObject->setPos(objectPosition);
	
	if (droppedObject != nullptr) {
		auto tr = currentObject->getGlobalPose();
		tr.q = rotation_droppedObject;
		currentObject->setGlobalPose(tr);
	}
}

void DroppingObjectsManager::drop() {
	if (currentObject == nullptr || !canBuild) return;

	rotation_droppedObject = currentObject->getGlobalPose().q;
	delete currentObject;
	currentObject = nullptr;

	droppedObject = new RigidSolid(gScene, gPhysics, *Models::static_dropping_solids[modelNum]);
	droppedObject->setPos(objectPosition);
	droppedObject->setRotation(rotation_droppedObject);

	static_piece_info* _info = new static_piece_info();
	_info->modelNum = modelNum;
	_info->q = rotation_droppedObject;
	_info->p = objectPosition;
	_info->solid = droppedObject;
	static_pieces.push_back(_info);

	currentPieces++;

	canBuild = currentPieces < maxPieces;

	if (!canBuild || timeToBuildEnded) {
		switchToDynamicPieces();
	}
	else generateRandomObject(position);
	// position.y += cameraUpMovement;

	// restartTimer();
	// stopMoving();
}

void DroppingObjectsManager::switchToDynamicPieces() {
	delete currentObject;
	currentObject = nullptr;
	list<RigidSolid*> solids;
	for (auto it = static_pieces.begin(); it != static_pieces.end(); ++it) {

		static_piece_info* _info = *it;
		RigidSolid* newSolid = new RigidSolid(gScene, gPhysics, *Models::dynamic_dropped_solids[_info->modelNum]);
		newSolid->setPos(_info->p);
		newSolid->setRotation(_info->q);
		solids.push_back(newSolid);
		dynamic_pieces.push_back(newSolid);


		delete _info->solid;
		_info->solid = nullptr;
	}
	particleSys->addParticles(solids);
	particleSys->addParticlesToRegistry(solids);

	canBuild = false;
	timeToBuildEnded = true;
	restartTimer();
}

void DroppingObjectsManager::switchToStaticPieces() {
	int i = 0;
	for (auto it = dynamic_pieces.begin(); it != dynamic_pieces.end() && i < static_pieces.size();) {

		static_piece_info* _info = static_pieces[i];
		RigidSolid* newSolid = new RigidSolid(gScene, gPhysics, *Models::static_dropping_solids[_info->modelNum]);
		newSolid->setPos((*it)->getPos());
		newSolid->setRotation((*it)->getGlobalPose().q);
		_info->solid = newSolid;

		delete (*it);
		(*it) = nullptr;
		it = dynamic_pieces.erase(it);

		++i;
	}
}

void DroppingObjectsManager::restartTimer() {
	timer = 0;
	timerActivated = true;
}

void DroppingObjectsManager::stopTimer() {
	timerActivated = false;
}

void DroppingObjectsManager::turnTimeHandler(double t) {
	if (!timerActivated) return;
	timer += t;
	if (!timeToBuildEnded && timer >= timeToBuild) {
		timeToBuildEnded = true;
		switchToDynamicPieces();
		restartTimer();
	}
	else if (timeToBuildEnded && timer >= postBuildTime) {
		buildPhaseEnded = true;
		stopTimer();
	}
}

void DroppingObjectsManager::update(double t) {
	moveHandler();

	if (currentObject != nullptr) currentObject->setPos(objectPosition);

	turnTimeHandler(t);
}

void DroppingObjectsManager::moveHandler() {
	if (!canMove) return;
	moveDir *= _lastWidth;
	moveDir.y *= 0.5f;
	position += moveDir;
	if (!_bb.insideBoundingBox(position)) position -= moveDir;
	moveDir = Vector3(0, 0, 0);
	objectPosition = position;
}