#include "Particle.h"
#include <list>
#include <string>
#include <random>
#include <chrono>
#include "ParticleGenerator.h"
#include <unordered_map>
using namespace std;

#pragma once
class ParticleSystem
{
private:
	list<Particle*> _particles;
	std::list<ParticleGenerator*> _particleGenerators;
	std::unordered_map<string, ParticleGenerator*> _particleGeneratorByName;
public:
	ParticleSystem() {};
	~ParticleSystem();
	void update(double t);
	void addGenerator(ParticleGenerator* _pG);
	ParticleGenerator* getGenerator(string name) { return _particleGeneratorByName[name]; };
};

