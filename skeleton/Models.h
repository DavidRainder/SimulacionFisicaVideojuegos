#pragma once
#include "Particle.h"
#include "RigidSolid.h"

namespace Models {
	static std::vector<Particle_config*> Fireworks {
		new Particle_config(0.998f, 2.5f, 1.0f, true, 1.0f),
		new Particle_config(0.998f, 3.5f, 0.5f, true, 1.0f),
		new Particle_config(0.998f, 4.5f, 0.5f, true, 1.0f),
		new Particle_config(0.998f, 5.5f, 1.5f, true, 1.0f),
		new Particle_config(0.998f, 5.0f, 1.0f, true, 1.0f),
		new Particle_config(0.998f, 4.0f, 2.5f, true, 1.0f),
		new Particle_config(0.998f, 3.0f, 1.0f, true, 1.0f),
		new Particle_config(0.998f, 4.5f, 1.5f, true, 1.0f),
		new Particle_config(0.998f, 6.0f, 1.8f, true, 1.0f),
		new Particle_config(0.998f, 2.8f, 1.0f, true, 1.0f),
		new Particle_config(0.998f, 3.8f, 1.0f, true, 1.0f),
		new Particle_config(0.998f, 4.3f, 1.0f, true, 1.0f)
	};

	static std::vector<Particle_config*> Springs{
		new Particle_config(0.998f, 240, .5f, false, 1.f, Sphere),
		new Particle_config(0.998f, 240, .5f, false, 1.f, Sphere),
		new Particle_config(0.998f, 240, .5f, false, 1.f, Sphere),
		new Particle_config(0.9f, 240, .5f, false, 1.f, Sphere),
		new Particle_config(0.9f, 240, 1.5f, false, 50.f, Sphere),
		new Particle_config(0.9f, 240, 5.0f, false, 500.f, Sphere)
	};

	static std::vector<Particle_config*> Water{
		new Particle_config(0.998f, 240, 1.0f, false, 1.0f, Plane, {0,.1f,1,1})
	};
		
	static std::vector<RigidSolid_config*> Solid{
		new RigidSolid_config(1.0f, DYNAMIC, BOX),
		new RigidSolid_config(1.0f, STATIC, BOX),
		new RigidSolid_config(1.0f, DYNAMIC, SPHERE),
		new RigidSolid_config(1.0f, STATIC, SPHERE),
	};
}
