#pragma once
#include "FireworkExplosionGenerator.h"
#include "Firework.h"
#include "Models.h"

FireworkGenerator::FireworkGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos, Vector3 variation) :
GaussianParticleGenerator(name, avgSpeed, avgPos, variation) {
	models = Models::Fireworks;
}

std::list<Particle*> FireworkGenerator::generateParticles(double t) {
	std::list<Particle*> _list;
	currentTime += t;
	if (currentTime >= timeTillNextParticle) {
		auto model = models[rand() % models.size()];
		model->vel = Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen));
		model->pos = _avgPos;
		Particle* newParticle = new Firework(3, *model);
		_list.push_back(newParticle);
		currentTime = 0;
	}
	return _list;
}

FireworkExplosionGenerator::FireworkExplosionGenerator(int generation, std::string name, Vector3 avgSpeed, Vector3 avgPos, int numParticles)
	: ParticleGenerator<Particle, Particle_config>(name, avgSpeed, avgPos), numParticles(numParticles), generation(generation) {
	models = Models::Fireworks;
}
std::list<Particle*> FireworkExplosionGenerator::generateParticles(double t)
{
	std::list<Particle*> _list;
	auto model = models[rand() % models.size()];
	for (int i = 0; i < numParticles; ++i) {
		model->pos = _avgPos;
		model->vel = (Vector3((float)(rand() % 100 - 50),
			(float)(rand() % 100 - 50), (float)(rand() % 100 - 50))).getNormalized() * 30;
		_list.push_back(new Firework(generation, *model));
	}
	Destroy();

	return _list;
}