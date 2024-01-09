#include "Particle.h"

class Firework : public Particle {
public:
	Firework(int generation, Vector3 Pos, Vector3 vel, bool usesGravity = false, float mass = 1.0f, float scale = 1, float damping = 0.98f, float maxTimeAlive = 3.0f);
	Firework(int generation, Vector3 Pos, Vector3 vel, Vector4 color = { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }, bool usesGravity = false, float mass = 1.0f, float scale = 1, float damping = 0.98f, float maxTimeAlive = 3.0f);
	Firework(int generation, Particle_config& _pC);

	void integrate(double t);
protected:
	Particle_config* _pC;
	int generation;

	virtual void Destroy();
};