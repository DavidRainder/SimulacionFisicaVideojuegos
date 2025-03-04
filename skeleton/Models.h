#pragma once
#include "Particle.h"
#include "RigidSolid.h"

namespace Models {
	static std::vector<Particle_config*> Fireworks {
		new Particle_config(0.998f, 1.f, 0.15f, true, 1.0f),
		new Particle_config(0.998f, 0.5f, 0.5f, true, 1.0f),
		new Particle_config(0.998f, 1.2f, 0.5f, true, 1.0f),
		new Particle_config(0.998f, 0.5f, 0.15f, true, 1.0f),
		new Particle_config(0.998f, 1.f, 0.5, true, 1.0f),
		new Particle_config(0.998f, 1.2f, 0.15f, true, 1.0f),
		new Particle_config(0.998f, 1.f, 0.5, true, 1.0f),
		new Particle_config(0.998f, 1.2f, 0.8, true, 1.0f),
		new Particle_config(0.998f, 0.5f, 0.15f, true, 1.0f),
		new Particle_config(0.998f, 1.f, 0.5, true, 1.0f),
		new Particle_config(0.998f, 1.2f, 0.15f, true, 1.0f),
		new Particle_config(0.998f, 1.f, 0.5, true, 1.0f)
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
		new Particle_config(0.998f, 240, 1.0f, false, 1.0f, Plane, {0,0.2f,0.4f,1})
	};
	
	static std::vector<RigidSolid_config*> Platform{
		new RigidSolid_config(10000000.0f, STATIC, PLATFORM, -1, 3, Vector4(0.4, 0.4, 0.4, 1)),
		new RigidSolid_config(10000000.0f, STATIC, PLATFORM, -1, 5, Vector4(0.4, 0.4, 0.4, 1))
	};

	static std::vector<RigidSolid_config*> Solid_Ground{
		new RigidSolid_config(10000000.0f, STATIC, GROUND, -1, 50, Vector4(0.5, 0.2, 0.4, 1)),
		new RigidSolid_config(10000000.0f, STATIC, GROUND, -1, 50, Vector4(0, 0.3, 0.3, 1)),
		new RigidSolid_config(10000000.0f, STATIC, GROUND, -1, 50, Vector4(0.3, 0.3, 0.6, 1)),
		new RigidSolid_config(10000000.0f, STATIC, GROUND, -1, 50, Vector4(0.3, 0, 0.3, 1)),
	};

	static std::vector<RigidSolid_config*> static_dropping_solids{
		new RigidSolid_config(1.0f, STATIC, SMALL_PIECE, -1, 1.0f, Vector4(0, 0, 0.3, 1.f)),
		new RigidSolid_config(1.0f, STATIC, MID_PIECE, -1, 1.0f, Vector4(0.3, 0, 0, 1.f)),
		new RigidSolid_config(1.0f, STATIC, LONG_PIECE, -1, 1.0f, Vector4(0, 0.3, 0, 1.f)),
	};

	static std::vector<RigidSolid_config*> dynamic_dropped_solids{
		new RigidSolid_config(1, DYNAMIC, SMALL_PIECE, -1, 1.0f, Vector4(0, 0, 0.3, 1.f)),
		new RigidSolid_config(1, DYNAMIC, MID_PIECE, -1, 1.0f, Vector4(0.3, 0, 0, 1.f)),
		new RigidSolid_config(1, DYNAMIC, LONG_PIECE, -1, 1.0f, Vector4(0, 0.3, 0, 1.f)),
	};

	static std::vector<RigidSolid_config*> Solid{
		new RigidSolid_config(1.0f, DYNAMIC, BOX, -1, 1.0f, Vector4(0.3, 0, 0, 0.2f)),
		new RigidSolid_config(1.0f, STATIC, BOX, -1),
		new RigidSolid_config(1.0f, DYNAMIC, SPHERE, -1),
		new RigidSolid_config(1.0f, STATIC, SPHERE, -1),
	};

	static std::vector<RigidSolid_config*> Solid_Projectiles{
		new RigidSolid_config(1.0f, DYNAMIC, SPHERE, 3.f, 0.8f, Vector4(0.2, 0.2, 0.2, 0.2f)),
	};
}
