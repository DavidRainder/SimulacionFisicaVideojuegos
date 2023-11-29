#include "SpringForceGenerator.h"
#pragma once
class AnchoredSpringFG : public SpringForceGenerator
{
public:
	AnchoredSpringFG(double k, double resting_length, const Vector3& anchored_pos) :
		SpringForceGenerator(k, resting_length, nullptr) {
		_other = new Particle(anchored_pos, { 0,0,0 }, true, 0.0f, 1.0f, .98f, -1);
	};

	~AnchoredSpringFG() { delete _other; }
};