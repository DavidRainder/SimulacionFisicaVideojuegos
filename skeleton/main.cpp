#pragma once
#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "ParticleSystem.h"
#include "GameManager.h"
#include "Plane.h"

#include "ForceGenerator.h"
#include "GravityForceGenerator.h"

#include "ParticleForceRegistry.h"
#include "ParticleDragGenerator.h"
#include "ExplosionForceGenerator.h"
#include "BouyancyForceGenerator.h"
#include "Whirlwind.h"
#include "GravityForceGenerator.h"

#include "RigidSolid.h"

#include <iostream>


std::string										display_text = "This is a test";

using namespace physx;

PxDefaultAllocator								gAllocator;
PxDefaultErrorCallback							gErrorCallback;

PxFoundation*									gFoundation = NULL;
PxPhysics*										gPhysics	= NULL;


PxMaterial*										gMaterial	= NULL;

PxPvd*											gPvd        = NULL;

PxDefaultCpuDispatcher*							gDispatcher = NULL;
PxScene*										gScene      = NULL;
ContactReportCallback							gContactReportCallback;

ParticleSystem<Particle, Particle_config>*		_pS;
ParticleSystem<RigidSolid, RigidSolid_config>*	_rSS;
ParticleForceRegistry<Particle>*				_pFR;

bool											paused = false;

RigidSolid* walls[4] {nullptr, nullptr, nullptr, nullptr};
RigidSolid* ground = nullptr;
RigidSolid* ceiling = nullptr;

ParticleForceRegistry<RigidSolid>* solid_rgst = nullptr;
ParticleForceRegistry<Particle>* particle_rgst = nullptr;

ParticleSystem<Particle, Particle_config>* particleSys = nullptr;
ParticleSystem<RigidSolid, RigidSolid_config>* solidSys = nullptr;

GameManager* gameManager = nullptr;

void initManagers() {
	solid_rgst = new ParticleForceRegistry<RigidSolid>();
	particle_rgst = new ParticleForceRegistry<Particle>();
	particleSys = new ParticleSystem<Particle, Particle_config>(gScene, particle_rgst);
	solidSys = new ParticleSystem<RigidSolid, RigidSolid_config>(gScene, solid_rgst);
	BouyancyForceGenerator<RigidSolid>* water_gen = new BouyancyForceGenerator<RigidSolid>(0.25f, 1.0f);
	solidSys->addForceGenerator(water_gen);
	GravityForceGenerator* gravityForce = new GravityForceGenerator(Vector3(0, -9.8f, 0));
	particleSys->addForceGenerator(gravityForce);
	gameManager = new GameManager(gScene, gPhysics, particleSys, solidSys, 2);

}

void initObjects() {
	ground = new RigidSolid(gScene, gPhysics, *Models::Solid_Ground[0]);
	walls[0] = new RigidSolid(gScene, gPhysics, *Models::Solid_Ground[1]);
	walls[0]->setPos(Vector3(30, 10, 0));
	walls[0]->rotate(90, Vector3(0, 0, 1));
	
	walls[1] = new RigidSolid(gScene, gPhysics, *Models::Solid_Ground[1]);
	walls[1]->setPos(Vector3(-30, 10, 0));
	walls[1]->rotate(90, Vector3(0, 0, 1));
	
	walls[2] = new RigidSolid(gScene, gPhysics, *Models::Solid_Ground[2]);
	walls[2]->setPos(Vector3(0, 10, -50));
	walls[2]->rotate(90, Vector3(1, 0, 0));
	
	walls[3] = new RigidSolid(gScene, gPhysics, *Models::Solid_Ground[2]);
	walls[3]->setPos(Vector3(0, 10, 50));
	walls[3]->rotate(90, Vector3(1, 0, 0));

	ceiling = new RigidSolid(gScene, gPhysics, *Models::Solid_Ground[3]);
	ceiling->setPos(Vector3(0, 50, 0));
}

void initGame() {
	initManagers();
	initObjects();
}

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);


	initGame();

}

void destroyAll() {
	delete gameManager;
	gameManager = nullptr;
	delete ground;
	ground = nullptr;
	delete walls[0];
	walls[0] = nullptr;
	delete walls[1];
	walls[1] = nullptr;
	delete walls[2];
	walls[2] = nullptr;
	delete walls[3];
	walls[3] = nullptr;
	delete particleSys;
	particleSys = nullptr;
	delete solidSys;
	solidSys = nullptr;
	delete solid_rgst;
	solid_rgst = nullptr;
	delete particle_rgst;
	particle_rgst = nullptr;
}

void restartGame() {
	destroyAll();
	initGame();
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);
	if (!paused) {
		particleSys->update(t);
		particle_rgst->updateForces(t);
		solid_rgst->updateForces(t);
		gameManager->update(t);
	}

	if (gameManager->getDestroy()) {
		restartGame();
	}

	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	destroyAll();
	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
		case 'P':
			paused = !paused;
			break;
		default: 
			break;
	}

	gameManager->keyPressed(key, camera);
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	gameManager->onCollision(actor1, actor2);

	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}

int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}