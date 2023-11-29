#include "ForceGenerator.h"

#pragma once
class SpringForceGenerator : public ForceGenerator
{
public:
	SpringForceGenerator(double k, double resting_length, Particle* other);

	virtual void updateForce(Particle* p, double duration);

	inline void setK(double k) { _k = k; }

protected:
	double _k; // elastic coeff
	double _resting_length;
	Particle* _other;
};

