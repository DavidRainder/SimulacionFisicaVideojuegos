#include "ParticleGenerator.h"

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
		T* newParticle = new T(_scene, *model);
		_list.push_back(newParticle);
		return _list;
	}

protected:
	std::default_random_engine gen;

	std::normal_distribution<float>* vX;
	std::normal_distribution<float>* vY;
	std::normal_distribution<float>* vZ;
};

