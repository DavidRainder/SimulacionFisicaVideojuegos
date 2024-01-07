#include "RenderUtils.hpp"
#include "ParticleGenerator.h"
#pragma once

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
	RigidSolid(physx::PxScene* scene, physx::PxPhysics* physics, RigidSolid_config& _config) : _type(_config.type), maxTimeAlive(_config.maxTimeAlive) {
		constructorAux(scene, physics, _config.shape, _config.pos, _config.vel, _config.density, _config.scale, _config.color);
	}
	RigidSolid(physx::PxScene* scene, physx::PxPhysics* physics, Solid_Type _type, Shape_Type _shape, Vector3 pos, Vector3 vel, float density, float maxTimeAlive = 3.f, float scale = 1.f,
		Vector4 color = {255,0,255,1.f}) : _type(_type), maxTimeAlive(maxTimeAlive) {
		constructorAux(scene, physics, _shape, pos, vel, density, scale, color);
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

	/// <summary>
	/// NUEVO MÉTODO:
	/// Referencia al PxTransform del sólido rígido
	/// </summary>

	physx::PxTransform getGlobalPose() {
		return _actor->getGlobalPose();
	}

	/// <summary>
	/// NUEVO MÉTODO:
	/// Settea el PxTransform del sólido rígido (para aplicar transformaciones)
	/// </summary>

	void setGlobalPose(physx::PxTransform& const tr) {
		_actor->setGlobalPose(tr);
	}
	
	/// <summary>
	/// NUEVO MÉTODO:
	/// Rota el sólido rígido según los parámetros dados; teniendo en cuenta la rotación actual
	/// </summary>
	/// <param name="deg_angle"> ángulo en grados </param>
	/// <param name="rot_axis"> vector unitario que dice en dónde aplicar las rotaciones </param>
	void rotate(float deg_angle, const physx::PxVec3& rot_axis) {
		auto tr = _actor->getGlobalPose();
		float quat_angle = (deg_angle) * (PI / 180);
		float sin_quat_angle = sin(quat_angle / 2);
		float nx = rot_axis.x * sin_quat_angle;
		float ny = rot_axis.y * sin_quat_angle;
		float nz = rot_axis.z * sin_quat_angle;
		float nw = cos(quat_angle / 2);

		tr.q = physx::PxQuat(nx, ny, nz, nw) * tr.q;
		_actor->setGlobalPose(tr);
	}

	/// <summary>
	/// NUEVO MÉTODO
	/// Settea la posición del Sólido Rígido a la posición global dada
	/// </summary>
	/// <param name="newPos"> Nueva posición global </param>
	void setPos(Vector3 newPos) {
		auto tr = _actor->getGlobalPose();
		tr.p = newPos;
		_actor->setGlobalPose(tr);
	}

	void setRotation(physx::PxQuat q) {
		auto tr = _actor->getGlobalPose();
		tr.q = q;
		_actor->setGlobalPose(tr);
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

	Vector3 halfExtents() { return Vector3(width, height, length); }
	Vector3 extents() { return Vector3(width * 2, height * 2, length * 2); }

	float getWidth() { return width * 2; }
	float getHeight() { return height * 2; }
	float getLength() { return length * 2; }

private:

	float width, height, length;

	void constructorAux(physx::PxScene* scene, physx::PxPhysics* physics, Shape_Type _shape, Vector3 pos, Vector3 vel, float density, float scale, Vector4 color) {
		physx::PxTransform* tr = new physx::PxTransform(pos);
		physx::PxShape* shape;
		if (_shape == SPHERE) {
			width = height = length = scale;
			physx::PxSphereGeometry sphere(width);
			shape = CreateShape(sphere);
		}
		else if (_shape == BOX) {
			width = height = length = scale;
			physx::PxBoxGeometry box(width, height, length);
			shape = CreateShape(box);
		}
		else if (_shape == PLANE) {
			width = length = scale;
			height = scale * 0.01f;
			physx::PxBoxGeometry box(width, height, length);
			shape = CreateShape(box);
		}
		else if (_shape == GROUND) {
			width = length = scale;
			height = scale * 0.01f;
			physx::PxBoxGeometry box(width, height, length);
			physx::PxMaterial* mMaterial;
			mMaterial = physics->createMaterial(1, 1, -1);
			shape = CreateShape(box, mMaterial);
		}
		else if (_shape == PLATFORM) {
			width = length = scale;
			height = 1;
			physx::PxBoxGeometry box(width, height, length);
			physx::PxMaterial* mMaterial;
			mMaterial = physics->createMaterial(1, 1, -1);
			shape = CreateShape(box, mMaterial);
		}
		else if (_shape == BOX_PIECE) {
			width = height = length = scale;
			physx::PxBoxGeometry box(width, height, length);
			physx::PxMaterial* mMaterial;
			mMaterial = physics->createMaterial(1, 1, -1);
			shape = CreateShape(box, mMaterial);
		}
		else if (_shape == LONG_PIECE) {
			width = height = scale;
			length = scale*3;
			physx::PxBoxGeometry box(width, height, length);
			physx::PxMaterial* mMaterial;
			mMaterial = physics->createMaterial(10000, 10000, -1);
			shape = CreateShape(box, mMaterial);
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
