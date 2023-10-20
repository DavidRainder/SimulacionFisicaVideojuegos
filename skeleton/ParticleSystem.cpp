#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Vector3 pos, Vector3 speed, Vector3 variation) : pos(pos), gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())) {
	//vX = new std::normal_distribution<float>(speed.x, variation.x);
	//vY = new std::normal_distribution<float>(speed.y, variation.y);
	//vZ = new std::normal_distribution<float>(speed.z, variation.z);
}

void ParticleSystem::update(double t) {

	for (auto it = _particles.begin(); it != _particles.end();) {
		if (!(*it)->getDestroy()) { (*it)->integrate(t); ++it; }
		else {
			delete (*it);
			it = _particles.erase(it);
		}
	}

	// _particles.push_back(new Particle(pos, Vector3((*vX)(gen),(*vY)(gen),(*vZ)(gen)), {0,0,0}, 0, 1.0f, 0.98f, 1.5f));
	for (auto it : _particleGenerators) {
		_particles.splice(_particles.end(), it->generateParticles());
	}
}