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
template<class T>
class ParticleSystem
{
private:
	const int MAX_PARTICLES = 5000;
	list<T*> _particles;
	std::list<ParticleGenerator<T>*> _particleGenerators;
	std::unordered_map<string, ParticleGenerator<T>*> _particleGeneratorByName;

	std::list<ForceGenerator<T>*> _forceGenerators;
	ParticleForceRegistry<T>* _pFR;
	std::list<SpringForceGenerator<T>*> _springGenerators;
public:
	ParticleSystem() : _pFR(new ParticleForceRegistry<T>()) {};
	~ParticleSystem() {
		for (auto it = _particles.begin(); it != _particles.end();) {
			_pFR->deleteParticleRegistry((*it));
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
				_pFR->deleteParticleRegistry((*it));
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
	}

	void addGenerator(ParticleGenerator<T>* _pG) { _particleGenerators.push_back(_pG); _particleGeneratorByName[_pG->getName()] = _pG; }

	void addForceGenerator(ForceGenerator<T>* _fG) { 
		_forceGenerators.push_back(_fG);
		for (auto it : _particles) {
			_pFR->addRegistry(_fG, it);
		}
	}

	void addParticlesToRegistry(list<Particle*> particles) {
		for (auto it : _forceGenerators) {
			for (auto ot : particles) {
				_pFR->addRegistry(it, ot);
			}
		}
	}

	ParticleGenerator<T>* getGenerator(string name) { return _particleGeneratorByName[name]; };

	void generateSpring() {
		// 2 particles
		Particle_config model = *Models::Springs[0];
		model.vel = { 0,0,0 };
		model.pos = Vector3(25, 50, 0);
		Particle* p1 = new Particle(model);
		model = *Models::Springs[1];
		model.pos = Vector3(25, 70, 0);
		Particle* p2 = new Particle(model);
		SpringForceGenerator<T>* _spring1 = new SpringForceGenerator<T>(1, 10, p2);
		_pFR->addRegistry(_spring1, p1);
		SpringForceGenerator<T>* _spring2 = new SpringForceGenerator<T>(1, 10, p1);
		_pFR->addRegistry(_spring2, p2);
		_forceGenerators.push_back(_spring1);
		_forceGenerators.push_back(_spring2);
		_springGenerators.push_back(_spring1);
		_springGenerators.push_back(_spring2);
		_particles.push_back(p1);
		_particles.push_back(p2);

		// fixed
		model = *Models::Springs[2];
		model.pos = { 50, 65, 0 };
		Particle* p3 = new Particle(model);
		AnchoredSpringFG<T>* _aSpring = new AnchoredSpringFG<T>(1, 30, { 50,75,0 });
		_pFR->addRegistry(_aSpring, p3);
		_particles.push_back(p3);
		_forceGenerators.push_back(_aSpring);
		_springGenerators.push_back(_aSpring);

		// Bouyancy
		model = *Models::Springs[3];
		model.pos = { 0, 10 ,0 };
		Particle* p4 = new Particle(model);
		model = *Models::Springs[4];
		model.pos = { 30, 10 ,0 };
		Particle* p5 = new Particle(model);
		model = *Models::Springs[5];
		model.pos = { -30, 10 ,0 };
		Particle* p6 = new Particle(model);
		_particles.push_back(p4);
		_particles.push_back(p5);
		_particles.push_back(p6);

		BouyancyForceGenerator<T>* _water = new BouyancyForceGenerator<T>(0, 1000);
		_forceGenerators.push_back(_water);
		_pFR->addRegistry(_water, p4);
		_pFR->addRegistry(_water, p5);
		_pFR->addRegistry(_water, p6);
	}

	void changeConstantSpring(double k) {
		for (auto it : _springGenerators) {
			it->setK(k);
		}
	}
};