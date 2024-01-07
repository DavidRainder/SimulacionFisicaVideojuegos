#include "core.hpp"
#include "ParticleGenerator.h"
#include "Models.h"
#include "BoundingBox.h"
#pragma once

class DroppingObjectsManager {
private:

	struct static_piece_info {
		int modelNum;
		physx::PxQuat q;
		Vector3 p;
		RigidSolid* solid;
	};

	#pragma region _CAM
	Vector3 cameraPositions[4]{
	Vector3(10.f, 6.f, -5.) * 2,
	Vector3(-10.f, 6.f, -5.) * 2,
	Vector3(-10.f, 6.f, 5.) * 2,
	Vector3(10.f, 6.f, 5.) * 2
	};

	Vector3 cameraDirections[4]{
		Vector3(-1, -0.5f, 0.5f),
		Vector3(1, -0.5f, 0.5f),
		Vector3(1, -0.5f, -0.5f),
		Vector3(-1, -0.5f, -0.5f)
	};

	Vector3 moveDirections[4]{
		Vector3(-1,0,0),
		Vector3(1,0,0),
		Vector3(0,0,1),
		Vector3(0,0,-1)
	};

	int currentCameraPosition = 0;
	#pragma endregion
	RigidSolid* platform;
	BoundingBox _bb;
	Vector3 position, objectPosition;
	Vector3 moveDir;
	float cameraUpMovement = 0;
	RigidSolid* currentObject = nullptr;
	RigidSolid* droppedObject = nullptr;
	physx::PxQuat rotation_droppedObject;
	physx::PxScene* gScene;
	physx::PxPhysics* gPhysics;

	float _lastWidth = 0;
	int modelNum = 0;
	float timer = 0;
	const float _time_between_pieces = 0.75f;
	float _objectUpSpeed = 0.0001f;

	bool timerActivated = false;
	bool canMove = true;

	std::vector<static_piece_info*> static_pieces;
	std::vector<RigidSolid*> dynamic_pieces;

	int currentPieces = 0;
	int maxPieces = 24;

	bool canBuild = true;

public:
	DroppingObjectsManager(physx::PxScene* scene, physx::PxPhysics* physics, Vector3 pos);

	// input handling
	void keyPressed(unsigned char key, physx::PxTransform Camera);

	// frame by frame behaviour
	void update(double t);

private:
	// piece generation
	void generateRandomObject(Vector3 pos);
	void drop();
	void undoDrop();

	// movement
	void moveHandler();
	inline void stopMoving() { canMove = false; };
	inline void startMoving() { canMove = true; };

	// rotation
	void handleRotation(int dir);

	// timers
	void restartTimer();
	void stopTimer();

	// switch pieces
	void switchToDynamicPieces();
};
