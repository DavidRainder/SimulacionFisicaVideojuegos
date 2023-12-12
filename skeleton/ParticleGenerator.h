#pragma once
#include "Particle.h"
#include <exception>
#include <list>
#include <random>
#include <chrono>
#include "Models.h"

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

template <class T, class Model_Config>
class GaussianParticleGenerator : public ParticleGenerator<T, Model_Config> {
public:
	GaussianParticleGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos, Vector3 variation) :
		ParticleGenerator<T, Model_Config>(name, avgSpeed, avgPos), gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())) {
		vX = new std::normal_distribution<float>(avgSpeed.x, variation.x);
		vY = new std::normal_distribution<float>(avgSpeed.y, variation.y);
		vZ = new std::normal_distribution<float>(avgSpeed.z, variation.z);
	}
	std::list<T*> generateParticles(double t) override {
		std::list<T*> _list;
		auto model = models[rand() % models.size()];
		model->vel = Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen));
		model->pos = _avgPos;
		T* newParticle = new T(nullptr, *model);
		_list.push_back(newParticle);
		return _list;
	}

protected:
	std::default_random_engine gen;

	std::normal_distribution<float>* vX;
	std::normal_distribution<float>* vY;
	std::normal_distribution<float>* vZ;
};

template <class T, class Model_Config>
class FireworkExplosionGenerator : public ParticleGenerator<T, Model_Config> {
public:
	FireworkExplosionGenerator(int generation, std::string name, Vector3 avgSpeed, Vector3 avgPos, int numParticles)
		: ParticleGenerator<T, Model_Config>(name, avgSpeed, avgPos), numParticles(numParticles), generation(generation) {
		models = Models::Fireworks;
	}
	std::list<Particle*> generateParticles(double t) {
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

protected:
	int generation;
	int numParticles;
};

class FireworkGenerator : public GaussianParticleGenerator<Particle, Particle_config> {
public:
	FireworkGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos, Vector3 variation) :
		GaussianParticleGenerator(name, avgSpeed, avgPos, variation) {
		models = Models::Fireworks;
	}

	std::list<Particle*> generateParticles(double t) {
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

private:
	const float timeTillNextParticle = 0.5f;
	float currentTime = 0;
};

template <class T, class Model_Config>
class UniformGenerator : public ParticleGenerator<T, Model_Config> {
public:

	UniformGenerator(std::string name, Vector3 avgPos, Vector3 posDesv, Vector3 avgVel, Vector3 velDesv) :
		ParticleGenerator<T, Model_Config>(name, avgVel, avgPos), gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())) {

		Vector3 pDesv = posDesv / 2;
		Vector3 vDesv = velDesv / 2;

		posX = new std::uniform_real_distribution<float>(-pDesv.x, pDesv.x);
		posY = new std::uniform_real_distribution<float>(-pDesv.y, pDesv.y);
		posZ = new std::uniform_real_distribution<float>(-pDesv.z, pDesv.z);

		velX = new std::uniform_real_distribution<float>(-vDesv.x, vDesv.x);
		velY = new std::uniform_real_distribution<float>(-vDesv.y, vDesv.y);
		velZ = new std::uniform_real_distribution<float>(-vDesv.z, vDesv.z);
	}

	std::list<T*> generateParticles(double t) {
		std::list<T*> _list;
		auto model = models[rand() % models.size()];
		model->pos = Vector3(_avgPos + Vector3((*posX)(gen), (*posY)(gen), (*posZ)(gen)));
		// model->vel = Vector3(_avgSpeed + Vector3((*velX)(gen), (*velY)(gen), (*velZ)(gen)));
		// aquí tengo que meter la escena, pero no sé cómo hehe
		_list.push_back(new T(_scene, *model));
		return _list;
	}
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