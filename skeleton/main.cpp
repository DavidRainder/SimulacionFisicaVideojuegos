#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Projectile.h"
#include "ParticleSystem.h"
#include "Plane.h"

#include "ForceGenerator.h"
#include "GravityForceGenerator.h"

#include "ParticleForceRegistry.h"
#include "ParticleDragGenerator.h"
#include "Whirlwind.h"
#include "ExplosionForceGenerator.h"
#include "BouyancyForceGenerator.h"

#include "RigidSolid.h"

#include <iostream>

std::string display_text = "This is a test";

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

ParticleSystem<Particle, Particle_config>* _pS;
ParticleSystem<RigidSolid, RigidSolid_config>* _rSS;
ParticleForceRegistry<Particle>* _pFR;

bool paused = false;

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

	GetCamera()->getTransform().rotate(Vector3(0, 0, 0));

	_pS = new ParticleSystem<Particle, Particle_config>(gScene);
	_rSS = new ParticleSystem<RigidSolid, RigidSolid_config>(gScene);

	RigidSolid* _ground = new RigidSolid(gScene, *Models::Solid_Ground[0]);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);
	if (!paused) {
		_pS->update(t);
		_rSS->update(t);
	}

#pragma region projectiles
	//for (auto it = projectiles.begin(); it != projectiles.end();) {
	//	if (!(*it)->getDestroy()) {
	//		(*it)->integrate(t); ++it;
	//	}
	//	else
	//	{
	//		delete (*it);
	//		it = projectiles.erase(it);
	//	}
	//}
#pragma endregion

#pragma region particles
	//for (auto it = particles.begin(); it != particles.end();) {
	//	if ((*it)->getDestroy()) {
	//		delete (*it);
	//		it = particles.erase(it);
	//	}
	//	else {
	//		(*it)->integrate(t);
	//		++it;
	//	}
	//}
#pragma endregion


	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

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
	//case 'B': break;
	//case ' ':	break;
	case 'B':
	{		
		BoundingBox bb = BoundingBox(Point(-30, -30, -30), Point(30, 30, 30));
		_pS->addForceGenerator(new ExplosionForceGenerator<Particle>(Vector3(0, 50, 0), 100.0f, 200, bb, 10.0f));
		break;
	}
	case 'E': {
		BoundingBox smallBB = BoundingBox(Point(-50, -50, -50), Point(50, 50, 50));
		BoundingBox bigBB = BoundingBox(Point(-100, -100, -100), Point(100, 100, 100));
		_rSS->addForceGenerator(new WhirlwindForceGenerator<RigidSolid>(Vector3(0, 0, 0), 1.0f,
			bigBB, 10.0f));
		break;
	}
	case 'C': {
		BoundingBox BB = BoundingBox(Point(-100, 30, -100), Point(100, 60, 100));
		_pS->addForceGenerator(new ParticleDragGenerator<Particle>(Vector3(0, 15, -15), .5f, 0, BB, 10.0f));
		break;
	}
	case ' ': {
		_rSS->addForceGenerator(new ExplosionForceGenerator<RigidSolid>(Vector3(0, 10, 0), 10000.0f, 2000));
		break;
	}
	case 'G': {
		GaussianParticleGenerator<RigidSolid, RigidSolid_config>* _gen = new GaussianParticleGenerator<RigidSolid, RigidSolid_config>("rigidSolid_uniform",
			Vector3(0, 10, 0), Vector3(0,10,0), Vector3(5.f, 1, 5.f));
		_gen->setParticleModel(Models::Solid[0]);
		_gen->setParticleModel(Models::Solid[2]);
		_rSS->addGenerator(_gen);
		break;
	}
	case 'Z': {
		UniformGenerator<RigidSolid, RigidSolid_config>* _gen = new UniformGenerator<RigidSolid, RigidSolid_config>("rigidSolid_uniform",
			Vector3(0, 10, 0), Vector3(10, .1f, 10), Vector3(.1f, 20, .1f), Vector3(.1f, 5, .1f));
		_gen->setParticleModel(Models::Solid[0]);
		_gen->setParticleModel(Models::Solid[2]);
		_rSS->addGenerator(_gen);
		break;
	}
	case 'K':
		_pS->generateSpring();
		break;
	case 'P':
		paused = !paused;
		break;
	case '+':
		paused = true;
		std::cout << "New K constant: ";
		double k;
		cin >> k;
		_pS->changeConstantSpring(k);
		paused = false;
		break;
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
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