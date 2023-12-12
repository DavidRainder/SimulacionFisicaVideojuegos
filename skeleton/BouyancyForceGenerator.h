#pragma once
#include "ForceGenerator.h"

template<class T>
class BouyancyForceGenerator : public ForceGenerator<T>
{
public:
    virtual ~BouyancyForceGenerator() {};

    BouyancyForceGenerator(float h, float d) :
        ForceGenerator(), _liquid_density(d) {
        Particle_config model = *Models::Water[0];
        model.pos = Vector3(0, h, 0);
        _liquid_particle = new Particle(model);
    };

    void updateForce(T* particle, double t) override
    {
        float h = particle->getPos().y;
        float h0 = _liquid_particle->getPos().y;

        _height = particle->getScale() * 2;

        Vector3 f(0, 0, 0);
        float immersed = 0.0;
        if (h - h0 > _height * 0.5)
        {
            immersed = 0.0;
        }
        else if (h0 - h > _height * 0.5)
        {
            immersed = 1.0;
        }
        else
        {
            immersed = (h0 - h) / _height + 0.5;
        }
        f.y = _liquid_density * particle->getVolume() * immersed * 9.8;

        particle->addForce(f);
    }

protected:
    float _height;
    float _liquid_density;

    Particle* _liquid_particle;
};