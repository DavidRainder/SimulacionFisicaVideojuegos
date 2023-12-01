#include "SpringForceGenerator.h"
#pragma once
class AnchoredSpringFG : public SpringForceGenerator
{
public:
	AnchoredSpringFG(double k, double resting_length, const Vector3& anchored_pos) :
		SpringForceGenerator(k, resting_length, nullptr) {
		_other = new Particle(anchored_pos, { 0,0,0 }, Cube, true, 0.0f, .3f, .98f, -1);
	};

	~AnchoredSpringFG() { delete _other; }
};