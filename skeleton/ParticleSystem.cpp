#include "ParticleSystem.h"

void ParticleSystem::update(double t) {

	for (auto it = _particles.begin(); it != _particles.end();) {
		if (!(*it)->getDestroy()) { (*it)->integrate(t); ++it; }
		else {
			if ((*it)->generatesOnDeath())
				addGenerator((*it)->getPG());
			delete (*it);
			it = _particles.erase(it);
		}
	}

	for (auto it = _particleGenerators.begin(); it != _particleGenerators.end();) {
		if ((*it)->getDestroy()) {
			delete (*it);
			it = _particleGenerators.erase(it);
		}
		else {
			auto newList = (*it)->generateParticles(t);
			_particles.splice(_particles.end(), newList);
			++it;
		}
	}
}