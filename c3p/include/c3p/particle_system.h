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

  /// reset the location and velocity of each particle to initial values
  void reset();

  /// invert the velocity vector of each particle (basically reverse in time)
  void reverse();
  
  /// add a new particle to the system
  void add(Particle &&);

  void remove(Particle &p);

  /// calculate the new velocity and location from the acceleration of all the particles 
  void update();

  /// change the exponent of the gravitational constant for the particle system
  void setGexponent(int exp);
  
  ///request a change in size of the system, fulfilled at next reset
  void requestParticles(size_t size);

  /// read gravitational constant
  float g_constant() const;

  /// return a reference to the particle container (where all the particles are stored)
  ParticleContainer &particles();

  const ParticleContainer &particles() const;

  bool empty() const;

  size_t size() const;

  ParticleContainer::iterator begin();

  ParticleContainer::iterator end();

 private:
  ParticleContainer _particles;
  float _G;  // the gravatational constant of the system
  size_t _requested_size;
};

}  // namespace c3p

#endif
