#pragma once

#include <map>
#include "ForceGenerator.h"

class ParticleForceRegistry : public std::multimap<Particle*, ForceGenerator*> {
public:
	void updateForces(double t) {
		for (auto it = begin(); it != end(); ++it) {
			it->second->updateForce(it->first, t);
		}
	}

	void addRegistry(ForceGenerator* fG, Particle* p) {
		emplace(p, fG);
	}

	void deleteParticleRegistry(Particle* p) {
		erase(p); // i have changed the multimap parameters to make this method 
				  // much more efficient (O(logN) rather than O(N));
	}
};