#include "SpringForceGenerator.h"
#pragma once

template<class T>
class AnchoredSpringFG : public SpringForceGenerator<T>
{
public:
	AnchoredSpringFG(double k, double resting_length, const Vector3& anchored_pos) :
		SpringForceGenerator(k, resting_length, nullptr) {
		_other = new T(anchored_pos, { 0,0,0 }, Cube, true, 0.0f, 1.5f, .98f, -1);
	};

	~AnchoredSpringFG() { delete _other; }
};