#include "Particle.h"
#include <list>
#include <string>
#include <random>
#include <chrono>
#include "ParticleGenerator.h"
#include "ForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "ParticleForceRegistry.h"
#include <unordered_map>
using namespace std;

#pragma once
class ParticleSystem
{
private:
	const int MAX_PARTICLES = 5000;
	list<Particle*> _particles;
	std::list<ParticleGenerator*> _particleGenerators;
	std::unordered_map<string, ParticleGenerator*> _particleGeneratorByName;

	list<ForceGenerator*> _forceGenerators;
	ParticleForceRegistry* _pFR;
public:
	ParticleSystem() : _pFR(new ParticleForceRegistry()) {};
	~ParticleSystem();
	void update(double t);
	void addGenerator(ParticleGenerator* _pG);
	void addForceGenerator(ForceGenerator* _fG) { 
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
	ParticleGenerator* getGenerator(string name) { return _particleGeneratorByName[name]; };


	void generateSpring();
};

