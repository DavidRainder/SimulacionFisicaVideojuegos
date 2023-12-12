#include "RenderUtils.hpp"
#pragma once
enum Solid_Type { STATIC, DYNAMIC };
enum Shape_Type { SPHERE, BOX };

struct RigidSolid_config {
public:
	Vector3 pos;
	float density, scale;
	Solid_Type type;
	Shape_Type shape;
	Vector4 color;

	RigidSolid_config(RigidSolid_config& rS) {
		this->pos = rS.pos;
		this->density = rS.density;
		this->type = rS.type;
		this->shape = rS.shape;
		this->color = rS.color;
		this->scale = rS.scale;
	}

	RigidSolid_config(float density, Solid_Type _type, Shape_Type _shape, float scale = 1.0f, Vector4 color = {255,0,255,1.f})
		: density(density), type(_type), shape(_shape), pos(Vector3(0, 0, 0)), scale(scale), color(color) {}
	RigidSolid_config(Vector3 pos, float density, Solid_Type _type, Shape_Type _shape, float scale = 1.0f, Vector4 color = { 255,0,255,1.f })
		: density(density), type(_type), shape(_shape), pos(pos), scale(scale), color(color) {}
};

class RigidSolid {
public:
	// pos, vel, type, color, gravity, mass, 
	// scale, damping, maxtimealive
	RigidSolid(physx::PxScene* scene, RigidSolid_config& _config) : _type(_config.type) {
		constructorAux(scene, _config.shape, _config.pos, _config.density, _config.scale, _config.color);
	}
	RigidSolid(physx::PxScene* scene, Solid_Type _type, Shape_Type _shape, Vector3 pos, float density, float scale = 1.f, 
		Vector4 color = {255,0,255,1.f}) : _type(_type) {
		constructorAux(scene, _shape, pos, density, scale, color);
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
		return _rI->transform->p;
	}

	void integrate(double t) {};
	ParticleGenerator<RigidSolid, RigidSolid_config>* getPG() { return _pG; }
private:
	void constructorAux(physx::PxScene* scene, Shape_Type _shape, Vector3 pos, float density, float scale, Vector4 color) {
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
		switch (_type) {
		case Solid_Type::STATIC:
			_actor = PxGetPhysics().createRigidStatic(*tr);
			break;
		case Solid_Type::DYNAMIC:
			_body = PxGetPhysics().createRigidDynamic(*tr);
			_actor = _body;
			physx::PxRigidBodyExt::updateMassAndInertia(*_body, density);

			break;
		default:
			_actor = PxGetPhysics().createRigidStatic(*tr);
			break;
		}
		scene->addActor(*_body);
		_rI = new RenderItem(shape, _actor, color);
	}
	RenderItem* _rI;
	physx::PxRigidActor* _actor;
	physx::PxRigidBody* _body;

	bool destroy = false, _generatesOnDeath = false;

	ParticleGenerator<RigidSolid, RigidSolid_config>* _pG;
	Solid_Type _type;
};
