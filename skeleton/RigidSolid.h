#include "RenderUtils.hpp"

struct RigidSolid_config {
public: 
	Vector3 pos;
	Vector3 vel;
};

class RigidSolid {
public:
	enum Solid_Type { STATIC, DYNAMIC };
	enum Shape_Type { SPHERE, BOX };
	// pos, vel, type, color, gravity, mass, 
	// scale, damping, maxtimealive
	RigidSolid(physx::PxScene* scene, Solid_Type _type, Shape_Type _shape, Vector3 pos, float density, float scale = 1.f, 
		Vector4 color = {255,0,255,1.f}) : _type(_type) {
		physx::PxTransform* tr = new physx::PxTransform(pos);
		physx::PxShape* shape;
		if (_shape == RigidSolid::SPHERE) {

			physx::PxSphereGeometry sphere(scale);
			shape = CreateShape(sphere);
		}
		else if(_shape == RigidSolid::BOX){
			physx::PxBoxGeometry box(scale, scale, scale);
			shape = CreateShape(box);
		}

		switch (_type) {
		case Solid_Type::STATIC:
			PxGetPhysics().createRigidStatic(*tr);
			break;
		case Solid_Type::DYNAMIC:
			PxGetPhysics().createRigidDynamic(*tr);
			break;
		default:
			PxGetPhysics().createRigidStatic(*tr);
			break;
		}
		physx::PxRigidBodyExt::updateMassAndInertia(*_body, density);
		scene->addActor(*_body);
		_rI = new RenderItem(shape, _body, color);
	}

	void addForce(Vector3 force) {
		_body->addForce(force);
	}

	Vector3 getPos() {
		return _rI->transform->p;
	}

private:
	RenderItem* _rI;
	physx::PxRigidBody* _body;

	Solid_Type _type;
};