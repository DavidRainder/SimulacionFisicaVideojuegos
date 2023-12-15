#include "RenderUtils.hpp"
#pragma once
enum Solid_Type { STATIC, DYNAMIC };
enum Shape_Type { SPHERE, BOX, PLANE };

struct RigidSolid_config {
public:
	Vector3 pos;
	Vector3 vel;
	float density, scale, maxTimeAlive;
	Solid_Type type;
	Shape_Type shape;
	Vector4 color;

	RigidSolid_config(RigidSolid_config& rS) {
		this->pos = rS.pos;
		this->vel = rS.vel;
		this->density = rS.density;
		this->type = rS.type;
		this->shape = rS.shape;
		this->color = rS.color;
		this->scale = rS.scale;
		this->maxTimeAlive = rS.maxTimeAlive;
	}

	RigidSolid_config(float density, Solid_Type _type, Shape_Type _shape, float maxTimeAlive = 3.f, float scale = 1.0f, Vector4 color = {255,0,255,1.f})
		: density(density), type(_type), shape(_shape), pos(Vector3(0, 0, 0)), vel(Vector3(0,0,0)), scale(scale), maxTimeAlive(maxTimeAlive), color(color) {}
	RigidSolid_config(Vector3 pos, Vector3 vel, float density, Solid_Type _type, Shape_Type _shape, float maxTimeAlive = 3.f, float scale = 1.0f, Vector4 color = { 255,0,255,1.f })
		: density(density), type(_type), shape(_shape), pos(pos), vel(vel), scale(scale), color(color), maxTimeAlive(maxTimeAlive) {}
};

class RigidSolid {
public:
	// pos, vel, type, color, gravity, mass, 
	// scale, damping, maxtimealive
	RigidSolid(physx::PxScene* scene, RigidSolid_config& _config) : _type(_config.type), maxTimeAlive(_config.maxTimeAlive) {
		constructorAux(scene, _config.shape, _config.pos, _config.vel, _config.density, _config.scale, _config.color);
	}
	RigidSolid(physx::PxScene* scene, Solid_Type _type, Shape_Type _shape, Vector3 pos, Vector3 vel, float density, float maxTimeAlive = 3.f, float scale = 1.f,
		Vector4 color = {255,0,255,1.f}) : _type(_type), maxTimeAlive(maxTimeAlive) {
		constructorAux(scene, _shape, pos, vel, density, scale, color);
	}

	~RigidSolid() {
		if (_rI != nullptr) _rI->release();
		_rI = nullptr;
		if (_actor != nullptr) _actor->release();
		_actor = nullptr;
	}

	void addForce(Vector3 force) {
		if(_type == DYNAMIC) _body->addForce(force);
	}

	bool getDestroy() {
		return destroy;
	}

	bool generatesOnDeath() {
		return _generatesOnDeath;
	}

	Vector3 getPos() {
		return _actor->getGlobalPose().p;
	}

	Vector3 getVel() {
		if (_body != nullptr)
			return _body->getLinearVelocity();
		else return Vector3(0, 0, 0);
	}

	float getInvMass() {
		if (_body != nullptr)
			return _body->getInvMass();
		else return .0f;
	}

	void Destroy() { destroy = true; }

	void integrate(double t) {
		timer += t;
		if (maxTimeAlive > 0 && timer > maxTimeAlive) {
			Destroy();
		}
	};
	ParticleGenerator<RigidSolid, RigidSolid_config>* getPG() { return _pG; }
private:
	void constructorAux(physx::PxScene* scene, Shape_Type _shape, Vector3 pos, Vector3 vel, float density, float scale, Vector4 color) {
		physx::PxTransform* tr = new physx::PxTransform(pos);
		physx::PxShape* shape;
		if (_shape == SPHERE) {

			physx::PxSphereGeometry sphere(scale);
			shape = CreateShape(sphere);
		}
		else if (_shape == BOX) {
			physx::PxBoxGeometry box(scale, scale, scale);
			shape = CreateShape(box);
		}
		else if (_shape == PLANE) {
			physx::PxBoxGeometry box(scale, 0.01f, scale);
			shape = CreateShape(box);
		}

		switch (_type) {
		case Solid_Type::STATIC:
			_actor = PxGetPhysics().createRigidStatic(*tr);
			break;
		case Solid_Type::DYNAMIC:
			_body = PxGetPhysics().createRigidDynamic(*tr);
			_actor = _body;
			_body->setLinearVelocity(vel);
			physx::PxRigidBodyExt::updateMassAndInertia(*_body, density);

			break;
		default:
			_actor = PxGetPhysics().createRigidStatic(*tr);
			break;
		}
		_actor->attachShape(*shape);
		scene->addActor(*_actor);
		_rI = new RenderItem(shape, _actor, color);
	}
	RenderItem* _rI;
	physx::PxRigidActor* _actor = nullptr;
	physx::PxRigidBody* _body = nullptr;

	bool destroy = false, _generatesOnDeath = false;

	float timer = 0;

	float maxTimeAlive = 3.f;

	ParticleGenerator<RigidSolid, RigidSolid_config>* _pG;
	Solid_Type _type;
};
