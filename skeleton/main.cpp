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

std::vector<Projectile*> projectiles;
std::vector<Particle*> particles;
GaussianParticleGenerator* gen;
ParticleSystem* _pS;
ParticleForceRegistry* _pFR;

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

	_pS = new ParticleSystem();

	Vector3 gravity = Vector3(0.0f,-9.8f,0.0f);

	const int offset = 15;
	
	float speed = 35.0f;

	GaussianParticleGenerator* gausGen = new GaussianParticleGenerator("Gaussian1", Vector3(0, speed, 0), Vector3(-offset, 0, 0), Vector3(1, 1, 1));
	gausGen->setParticleModel(new Particle_config(0.898f, 10.5f, 1.0f, true, 10.0f));
	_pS->addGenerator(gausGen);

	GaussianParticleGenerator* gausGen2 = new GaussianParticleGenerator("Gaussian2", Vector3(0, speed, 0), Vector3(offset, 0, 0), Vector3(1, 1, 1));
	gausGen2->setParticleModel(new Particle_config(0.898f, 10.5f, 1.0f, true, 0.1f));
	_pS->addGenerator(gausGen2);

	GaussianParticleGenerator* gausGen3 = new GaussianParticleGenerator("Gaussian3", Vector3(0, speed, 0), Vector3(0, 0, -offset), Vector3(1, 1, 1));
	gausGen3->setParticleModel(new Particle_config(0.898f, 10.5f, 1.0f, true, 0.01f));
	_pS->addGenerator(gausGen3);

	GaussianParticleGenerator* gausGen4 = new GaussianParticleGenerator("Gaussian4", Vector3(0, speed, 0), Vector3(0, 0, offset), Vector3(1, 1, 1));
	gausGen4->setParticleModel(new Particle_config(0.898f, 10.5f, 1.0f, true, 1.0f));
	_pS->addGenerator(gausGen4);

	// UniformGenerator* uniGen = new UniformGenerator("Uniform1", Vector3(0, 0, 0), Vector3(25, 0, 25), Vector3(0, 25, 0), Vector3(3, 6, 3));
	// uniGen->setParticleModel(new Particle_config(0.898f, 3.5f, 1.0f, true, 1.0f));
	//_pS->addGenerator(uniGen);
	// _pS->addForceGenerator(new GravityForceGenerator(gravity));
	// _pS->addForceGenerator(new ParticleDragGenerator(Vector3(2.0f,0,0), 1.0f, 0.0f));
	// _pS->addForceGenerator(new WhirlwindForceGenerator(Vector3(0,0,0), 5.0f));

	// Plane* plane = new Plane(Vector3(0, -50, 0), Vector3(250, 10, 250));

	//gen = new FireworkGenerator("Firework1", { 0,50,0 }, { 0,0,0 }, { 1,10,1 });
	//_pS->addGenerator(gen);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	_pS->update(t);

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

	for (int i = 0; i < projectiles.size(); ++i) {
		delete projectiles[i];
	}

	projectiles.clear();

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
		_pS->addForceGenerator(new ExplosionForceGenerator(Vector3(0, 50, 0), 100.0f, 200, bb, 10.0f));
		break;
	}
	case 'E': {
		BoundingBox smallBB = BoundingBox(Point(-50, -50, -50), Point(50, 50, 50));
		BoundingBox bigBB = BoundingBox(Point(-100, -100, -100), Point(100, 100, 100));
		_pS->addForceGenerator(new WhirlwindForceGenerator(Vector3(0, 0, 0), 1.0f,
			bigBB, 10.0f));
		break;
	}
	case 'C': {
		BoundingBox BB = BoundingBox(Point(-100, 30, -100), Point(100, 60, 100));
		_pS->addForceGenerator(new ParticleDragGenerator(Vector3(0, 15, -15), .5f, 0, BB, 10.0f));
		break;
	}
	case ' ': {
		_pS->addForceGenerator(new ExplosionForceGenerator(Vector3(0, 50, 0), 100000.0f, 2000));
		break;
	}
	case 'G': {
		Vector3 gravity = Vector3(0.0f, -9.8f, 0.0f);
		_pS->addForceGenerator(new GravityForceGenerator(gravity));
		break;
	}
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