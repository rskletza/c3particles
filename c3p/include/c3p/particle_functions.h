#ifndef C3P__PARTICLE_FUNCTIONS__INCLUDED
#define C3P__PARTICLE_FUNCTIONS__INCLUDED

#include <functional>
#include <vector>

#include <glm/glm.hpp>

//#include <c3p/particle_system.h>
#include <c3p/force_matrix.h>
#include <c3p/newtonian_objects.h>

namespace c3p
{
using Force = glm::vec3;
using ParticleContainer = std::vector<Particle>;

// wrapper for force
//  glm::vec3 force(glm::vec3 force);

//  std::ostream & operator<<(std::ostream &os, const Force &f);

//  Particle particle(float);

// applies a force to a particle
Particle &operator<<(Particle &lhs, Force &&rhs);

// applies a force matrix to a particle system (this applies the appropriate
// force to each particle)
ParticleContainer &operator<<(ParticleContainer &lhs, ForceMatrix &rhs);

// calculates a force between two particles using the function ff
// when given the same particle twice, it returns the identity
template <class NObject>
Force calc_force(const NObject &p1, const NObject &p2,
                 std::function<Force(const Particle &, const Particle &)> ff);

// uses calc_force to calculate gravitational force between two particles
Force gravity(const Particle &p1, const Particle &p2,
              std::initializer_list<float> G);

// calculates the force between two particles if they were attached to a spring
// (initializer list is for Spring constructor)
Force spring(const Particle &p1, const Particle &p2,
             std::initializer_list<float> G);

// calculates the force between two particles as if they were attached with a
// constant spring that doesn't change with distance
Force simple_attract(const Particle &p1, const Particle &p2,
                     std::initializer_list<float> G);

// calc_force for p with each other p in ps and reduce (addition) to one force
// disadvantage: for each pair of particles, the force is calculated twice
Force accumulate(const Particle &p, const ParticleContainer &ps,
                 std::function<Force(const Particle &, const Particle &)> ff);

// use a pre-defined function to calculate the forces, pass initializer list for
// parameters
Force accumulate(const Particle &p, const ParticleContainer &ps,
                 std::initializer_list<float> params,
                 std::function<Force(const Particle &, const Particle &,
                                     std::initializer_list<float>)>
                     ff);

Force accumulate(std::vector<Force> forces);

// update the location and velocity of a particle per time step
void update(Particle &p);

}

#endif  // C3P__PARTICLE_FUNCTIONS__INCLUDED
