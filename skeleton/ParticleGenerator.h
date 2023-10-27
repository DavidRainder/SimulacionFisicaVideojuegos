#pragma once
#include "Particle.h"
#include <exception>
#include <list>
#include <random>
#include <chrono>

class ParticleGenerator
{
/// <summary>
/// Usamos Particle_config como modelos de partícula. Usamos este struct para construir las partículas directamente :)
/// </summary>
protected:
	bool destroy = false;
	std::string name;
	Vector3 _avgSpeed, _avgPos;
	int _numParticles;
	std::vector<Particle_config*> models;
	void Destroy() { destroy = true; }
public:
	void changePos(Vector3 pos) { _avgPos = pos; }
	inline bool getDestroy() { return destroy; }

	ParticleGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos) : name(name), _avgSpeed(avgSpeed),
		_avgPos(avgPos) {
	};
	int setParticleModel(Particle_config* model) {
		models.push_back(new Particle_config(*model));
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

	virtual std::list<Particle*> generateParticles(double t) = 0;
};

class GaussianParticleGenerator : public ParticleGenerator {
public:
	GaussianParticleGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos, Vector3 variation);

	std::list<Particle*> generateParticles(double t);

protected:
	std::default_random_engine gen;

	std::normal_distribution<float>* vX;
	std::normal_distribution<float>* vY;
	std::normal_distribution<float>* vZ;
};

class FireworkExplosionGenerator : public ParticleGenerator {
public:
	FireworkExplosionGenerator(int generation, std::string name, Vector3 avgSpeed, Vector3 avgPos, int numParticles);

	std::list<Particle*> generateParticles(double t);

protected:
	int generation;
	int numParticles;
};

class FireworkGenerator : public GaussianParticleGenerator {
public:
	FireworkGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos, Vector3 variation);

	std::list<Particle*> generateParticles(double t);

private:
	const float timeTillNextParticle = 0.5f;
	float currentTime = 0;
};


class UniformGenerator : public ParticleGenerator {
public:
	UniformGenerator(std::string name, Vector3 avgPos, Vector3 posDesv, Vector3 avgVel, Vector3 velDesv);

	std::list<Particle*> generateParticles(double t);
private:
	// Posición
	std::uniform_real_distribution<float>* posX;
	std::uniform_real_distribution<float>* posY;
	std::uniform_real_distribution<float>* posZ;

	// Velocidad
	std::uniform_real_distribution<float>* velX;
	std::uniform_real_distribution<float>* velY;
	std::uniform_real_distribution<float>* velZ;

	std::default_random_engine gen;
};