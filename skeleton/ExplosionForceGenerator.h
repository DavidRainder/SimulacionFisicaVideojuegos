#include "ForceGenerator.h"

class ExplosionForceGenerator : public ForceGenerator {
public:
	ExplosionForceGenerator(Vector3 pos, float explosionForce, float r, 
		BoundingBox bb, float duration = 1e18) : 
		ForceGenerator(bb), pos(pos), explosionForce(explosionForce), r(r), velocity(r) {}

	ExplosionForceGenerator(Vector3 pos, float explosionForce, float r, float duration = 1e18) :
		ForceGenerator(duration), pos(pos), explosionForce(explosionForce), r(r), velocity(r) {}
	virtual void updateForce(Particle* p, double t);

private:
	Vector3 pos, velocity;
	float explosionForce;
	float r;
	float time = 0.0f;
};