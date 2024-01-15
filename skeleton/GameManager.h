#pragma once
#include "Cannon.h"
#include "DroppingObjectsManager.h"
#include "ParticleForceRegistry.h"
#include "ExplosionForceGenerator.h"
#include "Particle.h"
#include "FireworkExplosionGenerator.h"
#include "ParticleSystem.h"
#include <queue>

class GameManager
{
private:
	enum State { Menu, Build, Cannon, End, PressToRestart };
	enum TextType { Start_Text, BuildControls_Text, BuildTime_Text, BuildPieces_Text, PlayerID_Text, CannonControls_Text
		, Winners_Text, Restart_Text };
	std::vector<std::string> texts_strings{
		"PULSA 'ESPACIO' PARA COMENZAR",
		"CONTROLES:\n\t - WASD: Mover bloque\n\t - Q / E: Rotar bloque\n\t - R: Cambiar eje de rotacion\n\t - Z / X: Subir / Bajar bloque\n\t - B: Deshacer movimiento\n\t - 1 / 2 / 3 / 4: Mover cámara\n\t - ESPACIO: Colocar bloque",
		"nn.nn",
		"nn/nn",
		"JUGADOR ",
		"CONTROLES:\n\t - ESPACIO: Lanzar bola de demolicion\n\t - Mantener Click Derecho: Cambiar la dirección de vista de la cámara",
		"WINNER: ",
		"PULSA 'ESPACIO' PARA VOLVER A JUGAR"
	};
	const int numTexts = 8;
	static std::vector<int> texts_index;
	State state = Menu;

	struct Player {
	private:
		ParticleSystem<Particle, Particle_config>* particleSystem = nullptr;
		ParticleSystem<RigidSolid, RigidSolid_config>* solidSystem = nullptr;

	public:
		DroppingObjectsManager* dropMngr = nullptr;
		FiringCannon* cannon = nullptr;
		bool isWinner = false;
		~Player() {
			delete dropMngr;
			dropMngr = nullptr;
			delete cannon;
			cannon = nullptr;
		}

		int points = 0;
		int numExplosions = 1;
		void updateDrop(double t) {
			dropMngr->update(t);
		}
		
		bool updateCannon(double t) {
			return cannon->update(t);
		}

		void setParticleSystem(ParticleSystem<Particle, Particle_config>* particleSystem,
			ParticleSystem<RigidSolid, RigidSolid_config>* solidSystem) {
			this->particleSystem = particleSystem; this->solidSystem = solidSystem;
		}

		void activateFireworks() {
			FireworkGenerator* fireworkGen1 = new FireworkGenerator("fireworkGen1", { 0,5,0 }, dropMngr->getPos() - Vector3(0,1,0), {2,2,2}, 1);
			FireworkGenerator* fireworkGen2 = new FireworkGenerator("fireworkGen2", { 0,10,0 }, dropMngr->getPos() + Vector3(2,-1,-2), {4,4,4}, 1);
			FireworkGenerator* fireworkGen3 = new FireworkGenerator("fireworkGen3", { 0,15,0 }, dropMngr->getPos() + Vector3(-2,-1,2), {6,6,6}, 1);

			particleSystem->addGenerator(fireworkGen1);
			particleSystem->addGenerator(fireworkGen2);
			particleSystem->addGenerator(fireworkGen3);
		}

		void generateExplosion(RigidSolid* solid, list<RigidSolid*>& pieces) {
			if (numExplosions > 2) return;
			ExplosionForceGenerator<RigidSolid>* explosion = new ExplosionForceGenerator<RigidSolid>(solid->getPos(), solid->getVel().magnitude()/2/numExplosions, 50, 0.8f);
			solidSystem->addForceGenerator(explosion);
			solidSystem->addParticlesToRegistry(pieces);
			ExplosionForceGenerator<Particle>* explosion_particle = new ExplosionForceGenerator<Particle>(solid->getPos(), solid->getVel().magnitude()/2/numExplosions, 30, 0.8f);
			FireworkExplosionGenerator* firework_explosion = new FireworkExplosionGenerator(0,
				"firework_explosion_cannon_", { 0,0,0 }, solid->getPos() + Vector3(0.1, 0.1, 0.1), 50);
			firework_explosion->setParticleModel(Models::Fireworks[0]);

			auto fireworks = firework_explosion->generateParticles(0);
			particleSystem->addParticles(fireworks);
			particleSystem->addForceGenerator(explosion_particle);
			particleSystem->addParticlesToRegistry(fireworks);
			numExplosions++;
		}

		void onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
			auto pieces = dropMngr->DynamicPieces();
			auto projectiles = cannon->FiredProjectiles();

			for (auto it : projectiles) {
				for (auto ot : pieces) {
					if ((it->getActor() == actor1 || it->getActor() == actor2) && (actor2 == ot->getActor() || actor1 == ot->getActor())) {
						generateExplosion(it, pieces);
					}
				}
			}
		}

		inline bool hasEndedBuildPhase() {
			return dropMngr->hasEndedBuildPhase();
		}

		inline bool hasEndedCannonPhase() {
			return cannon->hasEndedCannonPhase();
		}
	};

	std::vector<Player*> _players;

	int numPlayers = 2;
	int currentPlayer;

	float timeToBuild = 45.0f;

	physx::PxScene* scene = nullptr;
	physx::PxPhysics* physics = nullptr;

	float camLookingDir = 0.1;
	int cam_dir = 1;

	ParticleSystem<Particle, Particle_config>* particleSys;
	ParticleSystem<RigidSolid, RigidSolid_config>* solidSys;

	float restartTimer = 0;
	const float timeTillRestart = 2.0f;
	bool restarted = false;

	bool destroy = false;

public:
	GameManager(physx::PxScene* scene, physx::PxPhysics* physics, ParticleSystem<Particle, Particle_config>* particleSys,
		ParticleSystem<RigidSolid, RigidSolid_config>* solidSys, int numPlayers);

	~GameManager();

	void StartGame();

	void update(double t);

	void keyPressed(unsigned char key, const physx::PxTransform& camera);

	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);
	
	inline bool getDestroy() const { return destroy; }

private:
	void generatePlayers();
	void addTextToRender(TextType _type);
	void removeTextFromRender(TextType _type);
	void updateTextFromRender(TextType _type, std::string newText);
	void updateOnBuildTexts();
	void StartFinalPhase();
	void StartCannonPhase();
	void NextPlayer();
	void handleRestartText(double t);
	void handleCameraMovement(double t);
};