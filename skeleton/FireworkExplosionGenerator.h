#include "GaussianParticleGenerator.h"

class FireworkExplosionGenerator : public ParticleGenerator<Particle, Particle_config> {
public:
	FireworkExplosionGenerator(int generation, std::string name, Vector3 avgSpeed, Vector3 avgPos, int numParticles);
	std::list<Particle*> generateParticles(double t);

protected:
	int generation;
	int numParticles;
};

class FireworkGenerator : public GaussianParticleGenerator<Particle, Particle_config> {
public:
	FireworkGenerator(std::string name, Vector3 avgSpeed, Vector3 avgPos, Vector3 variation);
	std::list<Particle*> generateParticles(double t);
private:
	const float timeTillNextParticle = 0.5f;
	float currentTime = 0;
};