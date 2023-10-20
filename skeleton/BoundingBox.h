#pragma once
#include "RenderUtils.hpp"
using Point = Vector3;
class BoundingBox
{
private:
	Point minP;
	Point maxP;

public:
	BoundingBox(Point min, Point max) : minP(min), maxP(max) {};
	Point dimensions() {
		return maxP - minP;
	}
	
	Point center() { return dimensions() / 2; }
	Point topCenter() { return center() + Vector3(0, dimensions().y / 2, 0); }
	Point bottomCenter() { return center() - Vector3(0,dimensions().y / 2,0); }
	float width() { return dimensions().x; }
	float height() { return dimensions().y; }
	float depth() { return dimensions().z; }
	
	virtual bool insideBoundingBox(Point p) {
		return p.x >= minP.x && p.x <= maxP.x &&
			p.y >= minP.y && p.y <= maxP.y &&
			p.z >= minP.z && p.z <= maxP.z;
	}
};

class BoundingSphere : BoundingBox {
private:
	float radius;
public:
	BoundingSphere(Point p, int radius) : BoundingBox(p - Vector3(radius, 0, 0), p + Vector3(radius, 0, 0)), radius(radius) {}
	bool insideBoundingBox(Point p) {
		return (p - center()).magnitude() <= radius;
	}
};

