#ifndef C3P__PARTICLE_SYSTEM__INCLUDED
#define C3P__PARTICLE_SYSTEM__INCLUDED

#include <iostream>
#include <vector>

#include <c3p/newtonian_objects.h>
#include <c3p/particle_renderer.h>
//#include <c3p/particle_container.h>

namespace c3p
{
class ParticleRenderer;
class ParticleSystem;

// Apply a force to a particle object
// void applyForce(glm::vec3 force, Particle &p);

std::ostream &operator<<(std::ostream &os, const ParticleSystem &ps);

class ParticleSystem
{
  using ParticleContainer = std::vector<Particle>;
  using vec = glm::vec3;  // std::array<float, 3>;
  using self_t = ParticleSystem;

  friend ParticleRenderer;  // TODO find something better than friend

 public:
  ParticleSystem() = delete;

  ParticleSystem(size_t size);
  ~ParticleSystem();

  self_t operator=(const self_t &other);
  self_t operator=(self_t &&other);

  // Give each particle a random color and location. the velocity is set
  // perpendicular to the location vector (this is what causes the circular
  // movement around 0,0,0)
  void setRandom();

  void reset();

  // Update the values (location, velocity, etc) of all the particles)
  void update();

  // Apply some force to all the particles
  //  void applyForceAll(vec force);

  // Add an attractor to the system. It attracts all the particles with constant
  // strength
  //  void addAttractor(vec point, float strength);

  // Enable origin force --> particles are attracted to their origin
  //  void gravitateOrigin(float strength);

  // Simulate gravitational forces on all the particles
  //  void nbodyGravity();

  // Add a gravitational mass to the system
  //  void addGForce(vec position, float mass);

  void setGexponent(int exp);

  float g_constant() const;

  const ParticleContainer &particles() const;

  bool empty() const;

  size_t size() const;

  ParticleContainer::iterator begin();

  ParticleContainer::iterator end();

 private:
  ParticleContainer _particles;
  float _G;  // the gravatational constant of the system
};

}  // namespace c3p

#endif
