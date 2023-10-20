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
	std::default_random_engine gen;
	list<Particle*> _particles;
	//std::normal_distribution<float>* vX;
	//std::normal_distribution<float>* vY;
	//std::normal_distribution<float>* vZ;
	Vector3 pos;
	std::list<ParticleGenerator*> _particleGenerators;

public:
	ParticleSystem(Vector3 pos, Vector3 speed, Vector3 variation);
	void update(double t);
	inline void addGenerator(ParticleGenerator* _pG) { _particleGenerators.push_back(_pG); }
	ParticleGenerator* getGenerator(string name);
	// void generateFireworkSystem();
};

