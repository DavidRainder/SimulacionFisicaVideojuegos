#pragma once
#include "DroppingObjectsManager.h"

DroppingObjectsManager::DroppingObjectsManager(physx::PxScene* scene, physx::PxPhysics* physics, Vector3 pos) :
	position(pos), objectPosition(pos), moveDir(Vector3(0, 0, 0)), gScene(scene), gPhysics(physics),
	_bb(Point(-5,0,-5), Point(5,10000000,5)) {
	for (int i = 0; i < 4; ++i) {
		cameraPositions[i] += pos;
	}

	platform = new RigidSolid(gScene, gPhysics, *Models::Platform[1]);
	platform->setPos(Vector3(0, platform->getHeight()/2, 0));
	generateRandomObject(pos);
}

void DroppingObjectsManager::handleRotation(int dir) {
	if (currentObject == nullptr) return;
	if (dir > 0) dir = 1;
	else dir = -1;
	currentObject->rotate(dir*90, Vector3(0, 1, 0));
}

void DroppingObjectsManager::keyPressed(unsigned char key, physx::PxTransform Camera) {
	switch (key) {
	case 'w':
		moveDir += moveDirections[currentCameraPosition%2];
		break;
	case 's':
		moveDir += moveDirections[(currentCameraPosition + 1) % 2];
		break;
	case 'a':
		moveDir += moveDirections[(currentCameraPosition) % 2 + 2];
		break;
	case 'd':
		moveDir += moveDirections[(currentCameraPosition + 1) % 2 + 2];
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
	case ' ':
		drop();
		restartTimer();
		break;
	case '1':
		currentCameraPosition = 0;
		SetCamera(cameraPositions[0], cameraDirections[0]);
		break;
	case '2':
		currentCameraPosition = 3;
		SetCamera(cameraPositions[1], cameraDirections[1]);
		break;
	case '3':
		currentCameraPosition = 1;
		SetCamera(cameraPositions[2], cameraDirections[2]);
		break;
	case '4':
		currentCameraPosition = 2;
		SetCamera(cameraPositions[3], cameraDirections[3]);
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
	modelNum = rand() % Models::static_dropping_solids.size();
	currentObject = new RigidSolid(gScene, gPhysics, *Models::static_dropping_solids[modelNum]);
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

	if (!canBuild)
		switchToDynamicPieces();
	else generateRandomObject(position);
	// position.y += cameraUpMovement;

	// restartTimer();
	// stopMoving();
}

void DroppingObjectsManager::switchToDynamicPieces() {
	delete currentObject;
	currentObject = nullptr;
	for (auto it = static_pieces.begin(); it != static_pieces.end();) {

		static_piece_info* _info = *it;
		RigidSolid* newSolid = new RigidSolid(gScene, gPhysics, *Models::dynamic_dropped_solids[_info->modelNum]);
		newSolid->setPos(_info->p);
		newSolid->setRotation(_info->q);
		dynamic_pieces.push_back(newSolid);

		delete _info->solid;
		_info->solid = nullptr;
		
		it = static_pieces.erase(it);
	}
}

void DroppingObjectsManager::restartTimer() {
	timer = 0;
	timerActivated = true;
}

void DroppingObjectsManager::stopTimer() {
	timerActivated = false;
}

void DroppingObjectsManager::update(double t) {
	moveHandler();

	if (currentObject != nullptr) currentObject->setPos(objectPosition);
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