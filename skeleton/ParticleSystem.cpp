#include "ParticleSystem.h"

void ParticleSystem::update(double t) {
	if (_pFR->updateForces(t)) {
		for (auto it = _forceGenerators.begin(); it != _forceGenerators.end();)
		{
			if ((*it)->getDestroy()) {
				delete (*it);
				(*it) = nullptr;
				it = _forceGenerators.erase(it);
			}
			else ++it;
		}
	}
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
		else if (_particles.size() < MAX_PARTICLES) {
			auto newList = (*it)->generateParticles(t);
			addParticlesToRegistry(newList);
			_particles.splice(_particles.end(), newList);
			++it;
		}
		else ++it;
	}
	for (auto it : _forceGenerators) {
		it->updateTime(t);
	}
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

void ParticleSystem::generateSpring() {
	// 2 particles
	Particle_config model = *Models::Springs[0];
	model.vel = { 0,0,0 };
	model.pos = Vector3(0, 0, 0);
	Particle* p1 = new Particle(model);
	model.pos = Vector3(0, -100, 0);
	Particle* p2 = new Particle(model);
	SpringForceGenerator* _spring1 = new SpringForceGenerator(1, 10, p2);
	_pFR->addRegistry(_spring1, p1);
	SpringForceGenerator* _spring2 = new SpringForceGenerator(1, 10, p1);
	_pFR->addRegistry(_spring2, p2);
	_forceGenerators.push_back(_spring1);
	_forceGenerators.push_back(_spring2);
	_particles.push_back(p1); 
	_particles.push_back(p2);

	// fixed
	model.pos = { 100, 15, 0 };
	Particle* p3 = new Particle(model);
	AnchoredSpringFG* _aSpring = new AnchoredSpringFG(1, 10, { 100,25,0 });
	_pFR->addRegistry(_aSpring, p3);
	_forceGenerators.push_back(_aSpring);
	_particles.push_back(p3);
}