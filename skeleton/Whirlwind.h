#include "ParticleDragGenerator.h"

class WhirlwindForceGenerator : public ForceGenerator {
public:
	WhirlwindForceGenerator(Vector3 centerPos, const float k
		, BoundingBox bb, float duration = 1e18) :
		ForceGenerator(bb, duration), k(k), centerPos(centerPos) {};
	WhirlwindForceGenerator(Vector3 centerPos, const float k, float duration = 1e18) :
		ForceGenerator(duration), k(k), centerPos(centerPos) {};
	virtual void updateForce(Particle* particle, double t);
private:
	float k;
	Vector3 centerPos;
};
