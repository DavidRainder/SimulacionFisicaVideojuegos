#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acc, float scale, float damping, float maxTimeAlive) {
	this->vel = vel;
	pose = physx::PxTransform(pos);
	physx::PxSphereGeometry sphere(scale);
	physx::PxShape* shape = CreateShape(sphere);
	const Vector4 color = { 255, 0, 255, 1 };
	this->acc = acc;
	this->damping = damping;
	rI = new RenderItem(shape, &pose, color);
}

void Particle::integrate(double t) {
	pose.p += vel * t;
	vel += acc * t + g*t;
	vel *= pow(damping,t);
	timer += t;
	if (timer > maxTimeAlive) {
		Destroy();
	}
}