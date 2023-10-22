#include "Particle.h"
#include <list>
#include <string>
#include <random>
#include <chrono>
#include "ParticleGenerator.h"
using namespace std;

#pragma once
class ParticleSystem
{
	list<Particle*> _particles;
	std::list<ParticleGenerator*> _particleGenerators;

public:
	ParticleSystem() {};
	void update(double t);
	inline void addGenerator(ParticleGenerator* _pG) { _particleGenerators.push_back(_pG); }
	ParticleGenerator* getGenerator(string name);
	// void generateFireworkSystem();
};

