#ifndef C3P__PARTICLE_FUNCTIONS__INCLUDED
#define C3P__PARTICLE_FUNCTIONS__INCLUDED

#include <functional>
#include <vector>

#include <glm/glm.hpp>

#include <c3p/particle_system.h>

namespace c3p
{
  using Force = glm::vec3;
  using ParticleContainer = std::vector<Particle>;

  //applies a force to a particle
  Particle & operator<<(Particle & lhs, Force && rhs);
  
  //calculates a force between two particles using the function ff
  //when given the same particle twice, it returns the identity 
  Force calc_force(const Particle & p1, const Particle & p2, std::function<Force(const Particle &, const Particle &)> ff);

  //uses calc_force to calculate gravitational force between two particles
  Force gravity(Particle & p1, Particle & p2, double G);

  //calc_force for p with each other p in ps and reduce (addition) to one force
  //disadvantage: for each pair of particles, the force is calculated twice
  Force accumulate(const Particle & p, const ParticleContainer & ps, std::function<Force(const Particle &, const Particle &)> ff);

  Force accumulate(std::initializer_list<Force> forces);

  //use a force matrix to avoid calculating each force twice (see ForceMatrix for more info on how this is done)
//  Force accumulate(Particle p, ForceMatrix & fm);
}

#endif //C3P__PARTICLE_FUNCTIONS__INCLUDED
