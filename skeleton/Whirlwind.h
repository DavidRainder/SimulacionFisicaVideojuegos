#include "ParticleDragGenerator.h"

class WhirlwindForceGenerator : public ForceGenerator {
public:
	WhirlwindForceGenerator(Vector3 centerPos, const float k
		, BoundingBox bb = BoundingBox(Point(0, 0, 0), Point(0, 0, 0))) :
		ForceGenerator(bb), k(k), centerPos(centerPos) {};
	virtual void updateForce(Particle* particle, double t);
private:
	float k;
	Vector3 centerPos;
};
