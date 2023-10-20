#pragma once
#include "Particle.h"
#include <exception>
#include <list>
class ParticleGenerator
{

/// <summary>
/// Usamos Particle_config como modelos de partícula. Usamos este struct para construir las partículas directamente :)
/// </summary>
protected:
	std::string name;
	Vector3 _avgSpeed, _avgPos;
	int _numParticles;
	std::vector<Particle_config*> models;
public:
	ParticleGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos) : name(name), _avgSpeed(avgSpeed),
		_avgPos(avgPos) {
	};
	int setParticleModel(Particle_config* model) {
		models.push_back(model);
		return models.size() - 1;
	}
	void changeParticleModel(Particle_config* model, int index) {
		try {
			models[index] = model;
		}
		catch (std::exception e) {
			throw std::to_string(index) + " particle model doesn't exist on " + name + " generator.";
		}
	}

	virtual std::list<Particle*> generateParticles() = 0;
};