#include "Particle.h"
#include "ParticleGenerator.h"

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acc, bool usesGravity, float gravity, float scale, float damping, float maxTimeAlive) : Particle(pos, vel, acc, { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }, usesGravity, gravity,
		scale, damping, maxTimeAlive) {}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acc, Vector4 color, bool usesGravity, float gravity, float scale, float damping, float maxTimeAlive) {
	pose = physx::PxTransform(pos); // posición
	physx::PxSphereGeometry sphere(scale); // escala
	physx::PxShape* shape = CreateShape(sphere);
	this->vel = vel; // velocidad
	this->acc = acc; // aceleración
	this->usesGravity = usesGravity; // usa gravedad?
	this->g = gravity; // gravedad que usa
	this->damping = damping; // damping
	this->maxTimeAlive = maxTimeAlive; // máximo tiempo de vida
	rI = new RenderItem(shape, &pose, color); // color
}

Particle::Particle(const Particle_config& _pC) : Particle(_pC.pos, _pC.vel, _pC.acc, _pC.color, _pC.usesGravity, _pC.g, _pC.scale, _pC.damping, _pC.maxTimeAlive){}

void Particle::integrate(double t) {
	pose.p += vel * t;
	// usesGravity ? vel += acc * t + Vector3(0, g * t, 0) : vel += acc * t;

	if (usesGravity) vel += acc * t + Vector3(0, g * t, 0);
	else vel += acc * t;

	vel *= pow(damping,t);
	timer += t;
	if (timer > maxTimeAlive) {
		Destroy();
	}
}

Firework::Firework(int generation, Vector3 Pos, Vector3 vel, Vector3 acc, bool usesGravity, float gravity, float scale, float damping, float maxTimeAlive)
	: Particle(Pos, vel, acc, { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }, usesGravity, gravity,
		scale, damping, maxTimeAlive), generation(generation) {
	if (generation != 0) {
		generatesParticles = true;
		this->_pC = new Particle_config(Pos, vel, acc, damping, maxTimeAlive * 0.7f, scale, usesGravity, gravity);
		generator = new FireworkExplosionGenerator(generation - 1, "Firework " + std::to_string(Pos.x + acc.x), { 50,50,50 }, Pos, 10);
		generator->setParticleModel(this->_pC);
	}
};

Firework::Firework(int generation, Vector3 Pos, Vector3 vel, Vector3 acc, Vector4 color, bool usesGravity, float gravity, float scale, float damping, float maxTimeAlive)
	: Particle(Pos, vel, acc, { (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, (float)(rand() % 256) / 255, 1 }, usesGravity, gravity,
		scale, damping, maxTimeAlive), generation(generation) {
	if (generation != 0) {
		generatesParticles = true;
		this->_pC = new Particle_config(Pos, vel, acc, damping, maxTimeAlive * 0.7f, scale, usesGravity, gravity, color);
		generator = new FireworkExplosionGenerator(generation - 1, "Firework " + std::to_string(Pos.x + acc.x), { 50,50,50 }, Pos, 10);
		Particle_config newPC = Particle_config((*this->_pC));
		generator->setParticleModel(_pC);
	}
};

Firework::Firework(int generation, Particle_config& _pC) : 
	Particle(_pC.pos, _pC.vel, _pC.acc, _pC.color, _pC.usesGravity, _pC.g, _pC.scale, _pC.damping, _pC.maxTimeAlive * 0.7f), generation(generation) {
	if (generation != 0) {
		generatesParticles = true;
		this->_pC = new Particle_config(_pC);
		generator = new FireworkExplosionGenerator(generation - 1, "Firework " + std::to_string(_pC.pos.x + acc.x), { 50,50,50 }, _pC.pos, 10);
		generator->setParticleModel(this->_pC);
	}
};

void Firework::Destroy() {
	Particle::Destroy();
	if(generation != 0) generator->changePos(pose.p);
}

void Firework::integrate(double t) {
	Particle::integrate(t);
	rI->color.w = 0;
}