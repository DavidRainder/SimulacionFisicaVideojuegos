#pragma once
#include "Particle.h"
#include <exception>
#include <list>
#include <random>
#include <chrono>
#include "RigidSolid.h"

template<class T, class Model_Config>
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
	std::vector<Model_Config*> models;
	void Destroy() { destroy = true; }
	physx::PxScene* _scene = nullptr;
public:
	void changePos(Vector3 pos) { _avgPos = pos; }
	inline bool getDestroy() { return destroy; }
	inline std::string getName() { return name; }
	void setScene(physx::PxScene* scene) { _scene = scene; }

	ParticleGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos) : name(name), _avgSpeed(avgSpeed),
		_avgPos(avgPos) {
	};
	int setParticleModel(Model_Config* model) {
		models.push_back(new Model_Config(*model));
		return models.size() - 1;
	}
	void changeParticleModel(Model_Config* model, int index) {
		try {
			models[index] = model;
		}
		catch (std::exception e) {
			throw std::to_string(index) + " particle model doesn't exist on " + name + " generator.";
		}
	}

	virtual std::list<T*> generateParticles(double t) = 0;
};