#include "Particle.h"

class RigidSolid {
public:
	// pos, vel, type, color, gravity, mass, 
	// scale, damping, maxtimealive
	RigidSolid() {

	}

private:
	RenderItem* _rI;
	physx::PxRigidBody* _body;


};