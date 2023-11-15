#include "ParticleDragGenerator.h"

class WhirlwindForceGenerator : public ForceGenerator {
public:
	WhirlwindForceGenerator(Vector3 centerPos, const float windForce) :
		ForceGenerator(), windForce(windForce), centerPos(centerPos) {};
	virtual void updateForce(Particle* particle, double t);
private:
	float windForce;
	Vector3 centerPos;
};
