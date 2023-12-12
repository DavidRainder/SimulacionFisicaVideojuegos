#include "ForceGenerator.h"

#pragma once
template<class T>
class SpringForceGenerator : public ForceGenerator<T>
{
public:
	SpringForceGenerator(double k, double resting_length, T* other) :
		ForceGenerator(), _k(k), _resting_length(resting_length), _other(other) {};

	void updateForce(T* particle, double t) {
		Vector3 relative_pos_vector = _other->getPos() - particle->getPos();
		Vector3 force;

		const float length = relative_pos_vector.normalize();
		const float delta_x = length - _resting_length;

		force = relative_pos_vector * delta_x * _k;

		particle->addForce(force);
	}

	inline void setK(double k) { _k = k; }

protected:
	double _k; // elastic coeff
	double _resting_length;
	T* _other;
};

