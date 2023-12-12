#pragma once
#include <map>
#include "ForceGenerator.h"

template<class T>
class ParticleForceRegistry : public std::multimap<T*, ForceGenerator<T>*> {
public:
	bool updateForces(double t) {
		bool destroyGen = false;
		for (auto it = begin(); it != end(); ) {
			if (it->second != nullptr && !it->second->getDestroy()
				&& it->first != nullptr && !it->first->getDestroy())
			{
				it->second->updateForce(it->first, t);
				++it;
			}
			else if (it->second == nullptr || it->second->getDestroy())
			{
				destroyGen = true;
				it = erase(it);
			}
			else ++it;
		}
		return destroyGen;
	}

	void addRegistry(ForceGenerator<T>* fG, T* p) {
		emplace(p, fG);
	}

	void deleteParticleRegistry(T* p) {
		erase(p); // i have changed the multimap parameters to make this method 
				  // much more efficient (O(logN) rather than O(N));
	}
};