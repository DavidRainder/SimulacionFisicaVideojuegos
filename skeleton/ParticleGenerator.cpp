#include "ParticleGenerator.h"

std::list<Particle*> GaussianParticleGenerator::generateParticles(double t) {
	std::list<Particle*> _list;
	auto model = models[rand() % models.size()];
	model->vel = Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen));
	model->pos = _avgPos;
	Particle* newParticle = new Particle(*model);
	_list.push_back(newParticle);
	return _list;
}

GaussianParticleGenerator::GaussianParticleGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos, Vector3 variation) : 
	ParticleGenerator(name, avgSpeed, avgPos), gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())) {
	vX = new std::normal_distribution<float>(avgSpeed.x, variation.x);
	vY = new std::normal_distribution<float>(avgSpeed.y, variation.y);
	vZ = new std::normal_distribution<float>(avgSpeed.z, variation.z);
}

FireworkExplosionGenerator::FireworkExplosionGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos, int numParticles)
	: ParticleGenerator(name, avgSpeed, avgPos), numParticles(numParticles) {}

std::list<Particle*> FireworkExplosionGenerator::generateParticles(double t) {
	std::list<Particle*> _list;
	for (int i = 0; i < numParticles; ++i) {
		auto model = models[rand() % models.size()];
		model->pos = _avgPos;
		model->vel = (Vector3((float)(rand() % 100 - 50),
			(float)(rand() % 100 - 50), (float)(rand() % 100 - 50))).getNormalized() * 30;
		_list.push_back(new Particle(*model));
	}
	Destroy();

	return _list;
}
FireworkGenerator::FireworkGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos, Vector3 variation) :
	GaussianParticleGenerator(name, avgSpeed, avgPos, variation) {}

std::list<Particle*> FireworkGenerator::generateParticles(double t) {
	std::list<Particle*> _list;
	currentTime += t;
	if (currentTime >= timeTillNextParticle) {
		auto model = models[rand() % models.size()];
		model->vel = Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen));
		model->pos = _avgPos;
		Particle* newParticle = new Firework(*model);
		_list.push_back(newParticle);
		currentTime = 0;
	}
	return _list;
}