#pragma once
#include "Particle.h"
#include <list>
#include <string>
#include <random>
#include <chrono>
#include "ParticleGenerator.h"
#include "ForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "ParticleForceRegistry.h"
#include "BouyancyForceGenerator.h"
#include <unordered_map>
using namespace std;

#pragma once
template<class T, class Model_Config>
class ParticleSystem
{
private:
	const int MAX_PARTICLES = 2000;
	list<T*> _particles;
	std::list<ParticleGenerator<T, Model_Config>*> _particleGenerators;
	std::unordered_map<string, ParticleGenerator<T, Model_Config>*> _particleGeneratorByName;
	physx::PxScene* scene;

	int numParticles = 0;

	std::list<ForceGenerator<T>*> _forceGenerators;
	ParticleForceRegistry<T>* _pFR;
	std::list<SpringForceGenerator<T>*> _springGenerators;
public:
	ParticleSystem(physx::PxScene* _scene) : _pFR(new ParticleForceRegistry<T>()), scene(_scene) {};
	ParticleSystem(physx::PxScene* _scene, ParticleForceRegistry<T>* pFR) : scene(_scene) {
		_pFR = pFR;
	};
	~ParticleSystem() {
		for (auto it = _particles.begin(); it != _particles.end();) {
			if(_pFR != nullptr) _pFR->deleteParticleFromRegistry((*it));
			delete (*it);
			*it = nullptr;
			it = _particles.erase(it);
		}
		for (auto it = _particleGenerators.begin(); it != _particleGenerators.end();) {
			delete (*it);
			*it = nullptr;
			it = _particleGenerators.erase(it);
		}
	}

	void update(double t) {
		if (_pFR->updateForces(t)) {
			for (auto it = _forceGenerators.begin(); it != _forceGenerators.end();)
			{
				if ((*it)->getDestroy()) {
					delete (*it);
					(*it) = nullptr;
					it = _forceGenerators.erase(it);
				}
				else ++it;
			}
		}
		for (auto it = _particles.begin(); it != _particles.end();) {
			if (!(*it)->getDestroy()) { (*it)->integrate(t); ++it; }
			else {
				if ((*it)->generatesOnDeath())
					addGenerator((*it)->getPG());
				_pFR->deleteParticleFromRegistry((*it));
				delete (*it);
				*it = nullptr;
				it = _particles.erase(it);
			}
		}
		for (auto it = _particleGenerators.begin(); it != _particleGenerators.end();) {
			if ((*it)->getDestroy()) {
				delete (*it);
				*it = nullptr;
				it = _particleGenerators.erase(it);
			}
			else if (_particles.size() < MAX_PARTICLES) {
				auto newList = (*it)->generateParticles(t);
				addParticlesToRegistry(newList);
				_particles.splice(_particles.end(), newList);
				++it;
			}
			else ++it;
		}
		for (auto it : _forceGenerators) {
			it->updateTime(t);
		}
		numParticles = _particles.size();
	}

	void addGenerator(ParticleGenerator<T, Model_Config>* _pG) { _particleGenerators.push_back(_pG); _particleGeneratorByName[_pG->getName()] = _pG;
		_pG->setScene(scene);
	}

	void addForceGenerator(ForceGenerator<T>* _fG) { 
		_forceGenerators.push_back(_fG);
		for (auto it : _particles) {
			_pFR->addRegistry(_fG, it);
		}
	}

	void addParticlesToRegistry(list<T*> particles) {
		for (auto it : _forceGenerators) {
			for (auto ot : particles) {
				_pFR->addRegistry(it, ot);
			}
		}
	}
	
	void addParticles(list<T*> particles) {
		_particles.splice(_particles.end(), particles);
	}

	ParticleGenerator<T, Model_Config>* getGenerator(string name) { return _particleGeneratorByName[name]; };
};