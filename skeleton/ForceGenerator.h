#include "Particle.h"
#include "BoundingBox.h"
#pragma once
class ForceGenerator {
public:
	ForceGenerator(BoundingBox bb = BoundingBox(Point(0,0,0), Point(0,0,0)), float duration = 1e18) : _bb(bb), _duration(duration) {
		usesBB = bb.dimensions().magnitude() > 0;
	};
	ForceGenerator(float duration = 1e18) : _duration(duration), _bb(BoundingBox(Point(0, 0, 0), Point(0, 0, 0))) {
		usesBB = false;
	};
	virtual void updateForce(Particle* particle, double duration) = 0;
	std::string _name;
	double _t = 0.0; // If starting negative --> eternal
	double _duration = -1e10;
	inline bool updateTime(double t) {
		_t += t;
		dead = !(_t < _duration || _duration < 0.0);
		return dead; // Devuelve true si ya era cero o si es positivo
	}
	virtual ~ForceGenerator() {}

	inline bool getDestroy() { return dead; }
protected:
	virtual void Destroy() { dead = true; }
	bool dead = false;
	bool usesBB = false;
	BoundingBox _bb;
};
