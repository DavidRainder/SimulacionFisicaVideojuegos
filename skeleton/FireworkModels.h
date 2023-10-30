#pragma once
#include "Particle.h"

namespace Models {
	static std::vector<Particle_config*> Fireworks {
	new Particle_config(0.998f, 0.5f, 1.0f, true, -9.8f),
	new Particle_config(0.998f, 1.5f, 0.5f, true, -9.8f),
	new Particle_config(0.998f, 2.5f, 0.5f, true, -9.8f),
	new Particle_config(0.998f, 3.5f, 1.5f, true, -9.8f),
	new Particle_config(0.998f, 3.0f, 1.0f, true, -9.8f),
	new Particle_config(0.998f, 2.0f, 2.5f, true, -9.8f),
	new Particle_config(0.998f, 1.0f, 1.0f, true, -9.8f),
	new Particle_config(0.998f, 2.5f, 3.5f, true, -9.8f),
	new Particle_config(0.998f, 5.0f, 3.5f, true, -9.8f),
	new Particle_config(0.998f, 0.8f, 1.0f, true, -9.8f),
	new Particle_config(0.998f, 1.8f, 1.0f, true, -9.8f),
	new Particle_config(0.998f, 2.3f, 1.0f, true, -9.8f),
	};;
}
