#pragma once
#include <map>
#include "ForceGenerator.h"

class ParticleForceRegistry : public std::multimap<Particle*, ForceGenerator*> {
public:
	bool updateForces(double t) {
		bool destroyGen = false;
		for (auto it = begin(); it != end(); ) {
			if (!it->second->getDestroy()) { it->second->updateForce(it->first, t); ++it; }
			else { destroyGen = true; it = erase(it); }
		}
		return destroyGen;
	}

	void addRegistry(ForceGenerator* fG, Particle* p) {
		emplace(p, fG);
	}

	void deleteParticleRegistry(Particle* p) {
		erase(p); // i have changed the multimap parameters to make this method 
				  // much more efficient (O(logN) rather than O(N));
	}
};