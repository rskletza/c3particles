#ifndef C3P__PARTICLE_SYSTEM__INCLUDED
#define C3P__PARTICLE_SYSTEM__INCLUDED

#include <vector>

#include "include/particle_renderer.h"

namespace c3p {

class ParticleRenderer;

//partikel: zustand eines einzelnen partikels erhalten
struct Particle
{
    glm::vec3 color; //TODO: does this belong here?
    glm::vec3 origin;
    glm::vec3 location;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
    float ttl;
};

//apply a force
//concept von etwas, worauf gravitation wirken kann
void applyForce(glm::vec3 force, Particle & p);

class ParticleSystem
{
    using ParticleContainer = std::vector<Particle>;
    using vec = glm::vec3; //std::array<float, 3>;
    using self_t = ParticleSystem;

    friend ParticleRenderer; //TODO find something better than friend

  public:

    ParticleSystem() = delete;

    ParticleSystem(size_t size);
    ~ParticleSystem();

    self_t operator=(const self_t & other);
    self_t operator=(self_t && other);

    //give each particle a random color and location. the velocity is set perpendicular to the location vector (this is what causes the circular movement around 0,0,0)
    void setRandom();

    //update the values (location, velocity, etc) of all the particles)
    void update();

    //apply some force to all the particles
    void applyForceAll(vec force) const;

    //add an attractor to the system. It attracts all the particles with constant strength
    void addAttractor(vec point, float strength) const;

    //enable origin force --> particles are attracted to their origin
    void gravitateOrigin(float strength) const;

    //simulate gravitational forces on all the particles
    void nbodyGravity() const;

    //simulate spring forces between all the particles
    void nbodySprings(float tension) const;

    //add a gravitational mass to the system
    void addGForce(vec position, float mass) const;

    //print values of the particles
    void print() const;

    bool empty() const;

    size_t size() const;

  private:
    ParticleContainer * _particles;
    float _G; //the gravatational constant of the system
};

} //namespace c3p

#endif
