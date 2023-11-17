#include "Particle.h"
#include "BoundingBox.h"
#pragma once
class ForceGenerator {
public:
	ForceGenerator(BoundingBox bb = BoundingBox(Point(0,0,0), Point(0,0,0))) : _bb(bb) {
		usesBB = bb.dimensions().magnitude() > 0;
	};
	virtual void updateForce(Particle* particle, double duration) = 0;
	std::string _name;
	double _t = 0.0; // If starting negative --> eternal
	double _duration = -1e10;
	inline bool updateTime(double t) {
		_t += t;
		return _t < _duration || _duration < 0.0; // Devuelve true si ya era cero o si es positivo
	}
	virtual ~ForceGenerator() {}

	inline bool getDestroy() { return dead; }
protected:
	virtual void Destroy() { dead = true; }
	bool dead = false;
	bool usesBB = false;
	BoundingBox _bb;
};
