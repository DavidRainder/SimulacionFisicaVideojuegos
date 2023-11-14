#include "ParticleSystem.h"

void ParticleSystem::update(double t) {

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
		else {
			auto newList = (*it)->generateParticles(t);
			addParticlesToRegistry(newList);
			_particles.splice(_particles.end(), newList);
			++it;
		}
	}

	_pFR->updateForces(t);
}

void ParticleSystem::addGenerator(ParticleGenerator* _pG) { _particleGenerators.push_back(_pG); _particleGeneratorByName[_pG->getName()] = _pG; }


ParticleSystem::~ParticleSystem() {
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