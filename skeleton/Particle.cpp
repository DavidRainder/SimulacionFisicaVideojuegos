#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acc, bool usesGravity, float scale, float damping, float maxTimeAlive) {
	Particle(pos, vel, acc, usesGravity, { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 },
		scale, damping, maxTimeAlive);
}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acc, bool usesGravity, Vector4 color, float scale, float damping, float maxTimeAlive) {
	this->vel = vel;
	pose = physx::PxTransform(pos);
	physx::PxSphereGeometry sphere(scale);
	physx::PxShape* shape = CreateShape(sphere);
	const Vector4 color = color;
	this->acc = acc;
	this->damping = damping;
	rI = new RenderItem(shape, &pose, color);
	this->maxTimeAlive = maxTimeAlive;
}

void Particle::integrate(double t) {
	pose.p += vel * t;
	usesGravity ? vel += acc * t + Vector3(0, g * t, 0) : vel += acc * t;
	vel *= pow(damping,t);
	timer += t;
	if (timer > maxTimeAlive) {
		Destroy();
	}
}