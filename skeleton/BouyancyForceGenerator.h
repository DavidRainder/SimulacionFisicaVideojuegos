#pragma once
#include "ForceGenerator.h"

class BouyancyForceGenerator : public ForceGenerator
{
public:
    BouyancyForceGenerator(float h, float d);
    virtual void updateForce(Particle* particle, double t);

    virtual ~BouyancyForceGenerator() {};

protected:
    float _height;
    float _liquid_density;

    Particle* _liquid_particle;
};