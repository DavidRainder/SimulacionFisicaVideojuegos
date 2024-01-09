#include "ParticleGenerator.h"
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
		model->vel = Vector3(_avgSpeed + Vector3((*velX)(gen), (*velY)(gen), (*velZ)(gen)));
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