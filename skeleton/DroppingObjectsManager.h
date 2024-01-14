#include "core.hpp"
#include "ParticleGenerator.h"
#include "Models.h"
#include "BoundingBox.h"
#include "ParticleSystem.h"
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
	Vector3 cameraPositions[8]{
		Vector3(-5.f, 6.f, 10.f) * 2,
		Vector3(-5., 6.f, -10.f) * 2,
		Vector3(5., 6.f, -10.f) * 2,
		Vector3(5., 6.f, 10.f) * 2,
		Vector3(5., 6.f, -10.f) * 2,
		Vector3(5., 6.f, 10.f) * 2,
		Vector3(-5.f, 6.f, 10.f) * 2,
		Vector3(-5., 6.f, -10.f) * 2,
	};

	Vector3 cameraDirections[8]{
		Vector3(.5f, -0.5f, -1),
		Vector3(.5f, -0.5f, 1),
		Vector3(-0.5f, -0.5f, 1),
		Vector3(-0.5f, -0.5f, -1),
		Vector3(-0.5f, -0.5f, 1),
		Vector3(-0.5f, -0.5f, -1),
		Vector3(.5f, -0.5f, -1),
		Vector3(.5f, -0.5f, 1),
	};

	Vector3 moveDirections[8]{
		Vector3(0,0,-1),
		Vector3(0,0,1),
		Vector3(-1,0,0),
		Vector3(1,0,0),
		Vector3(0,0,1),
		Vector3(0,0,-1),
		Vector3(1,0,0),
		Vector3(-1,0,0),
	};

	int currentCameraPosition = 0;
	#pragma endregion
	Vector3 rotation_axis[3]{
		Vector3(0,0,1),
		Vector3(0,1,0),
		Vector3(1,0,0)
	};
	int currentRotation = 1;
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
	float timeToBuild = 0;
	float postBuildTime = 3.5f;
	bool timerActivated = false;
	bool timeToBuildEnded = false;

	int camOffset = 2;

	float _objectUpSpeed = 0.0001f;

	bool canMove = true;

	std::vector<int> avaliablePieces;
	std::vector<static_piece_info*> static_pieces;
	std::list<RigidSolid*> dynamic_pieces;

	int currentPieces = 0;
	int maxPieces = 24;

	bool canBuild = true;

	bool buildPhaseEnded = false;

	ParticleSystem<RigidSolid, RigidSolid_config>* particleSys = nullptr;
public:
	DroppingObjectsManager(physx::PxScene* scene, physx::PxPhysics* physics, ParticleSystem<RigidSolid, RigidSolid_config>* particleSys
		, Vector3 pos, float time, int seed = 1);
	~DroppingObjectsManager();

	Vector3 getPos() { return position; }

	// input handling
	void keyPressed(unsigned char key, physx::PxTransform Camera);

	// frame by frame behaviour
	void update(double t);

	void StartGame();

	void turnTimeHandler(double t);

	inline bool hasEndedBuildPhase() { return buildPhaseEnded; };

	// switch pieces
	void switchToDynamicPieces();
	void switchToStaticPieces();

	int numObjectsStillStanding();

	inline float getTimeLeftToBuild() {
		if(canBuild) return abs(timer - timeToBuild);
		return 0.0f;
	}

	inline int MaxPieces() {
		return maxPieces;
	}

	inline int NumPieces() {
		return currentPieces;
	}

	std::list<RigidSolid*> DynamicPieces() { return dynamic_pieces; };

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
};
