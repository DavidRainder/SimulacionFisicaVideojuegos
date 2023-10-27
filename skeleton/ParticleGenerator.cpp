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

FireworkExplosionGenerator::FireworkExplosionGenerator(int generation, std::string name, Vector3 avgSpeed, Vector3 avgPos, int numParticles)
	: ParticleGenerator(name, avgSpeed, avgPos), numParticles(numParticles), generation(generation) {}

std::list<Particle*> FireworkExplosionGenerator::generateParticles(double t) {
	std::list<Particle*> _list;
	for (int i = 0; i < numParticles; ++i) {
		auto model = models[rand() % models.size()];
		model->pos = _avgPos;
		model->vel = (Vector3((float)(rand() % 100 - 50),
			(float)(rand() % 100 - 50), (float)(rand() % 100 - 50))).getNormalized() * 30;
		_list.push_back(new Firework(generation,*model));
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
		Particle* newParticle = new Firework(3, *model);
		_list.push_back(newParticle);
		currentTime = 0;
	}
	return _list;
}

UniformGenerator::UniformGenerator(std::string name, Vector3 avgPos, Vector3 posDesv, Vector3 avgVel, Vector3 velDesv) :
	ParticleGenerator(name, avgVel, avgPos), gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())) {

	Vector3 pDesv = posDesv / 2;
	Vector3 vDesv = velDesv / 2;

	posX = new std::uniform_real_distribution<float>(-pDesv.x, pDesv.x);
	posY = new std::uniform_real_distribution<float>(-pDesv.y, pDesv.y);
	posZ = new std::uniform_real_distribution<float>(-pDesv.z, pDesv.z);

	velX = new std::uniform_real_distribution<float>(-vDesv.x, vDesv.x);
	velY = new std::uniform_real_distribution<float>(-vDesv.y, vDesv.y);
	velZ = new std::uniform_real_distribution<float>(-vDesv.z, vDesv.z);
}

std::list<Particle*> UniformGenerator::generateParticles(double t) {
	std::list<Particle*> _list;
	auto model = models[rand() % models.size()];
	model->pos = Vector3(_avgPos + Vector3((*posX)(gen), (*posY)(gen), (*posZ)(gen)));
	model->vel = Vector3(_avgSpeed + Vector3((*velX)(gen), (*velY)(gen), (*velZ)(gen)));
	_list.push_back(new Particle(*model));
	return _list;
}