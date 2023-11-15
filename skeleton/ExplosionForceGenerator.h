#include "ForceGenerator.h"

class ExplosionForceGenerator : public ForceGenerator {
public:
	ExplosionForceGenerator(Vector3 pos, float explosionForce, float r) : 
		ForceGenerator(), pos(pos), explosionForce(explosionForce), r(r) {}

	virtual void updateForce(Particle* p, double t);

private:
	Vector3 pos;
	float explosionForce;
	float r;
};