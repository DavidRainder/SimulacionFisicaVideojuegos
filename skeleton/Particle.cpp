#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel) {
	this->vel = Vel;
	pose = physx::PxTransform(Pos);
	physx::PxSphereGeometry sphere(10);
	physx::PxShape* shape = CreateShape(sphere);
	const Vector4 color = { 255, 0, 255, 1 };
	rI = new RenderItem(shape, &pose, color);
}

void Particle::integrate(double t) {
	pose.p += vel * t;
}