#include <c3p/particle_functions.h>

//#include <execution> c++17
#include <algorithm>
#include <iostream>
#include <numeric>

namespace c3p
{
// Some sites I used to brush up on my physics:
// http://www.pas.rochester.edu/~blackman/ast104/newton3laws16.html
// https://www.johannes-strommer.com/rechner/basics-mathe-mechanik/ruck-beschleunigung-geschwindigkeit-weg
// http://zonalandeducation.com/mstm/physics/mechanics/forces/newton/mightyFEqMA/mightyFEqMA.html
// http://physics.weber.edu/amiri/physics1010online/WSUonline12w/OnLineCourseMovies/CircularMotion&Gravity/reviewofgravity/ReviewofGravity.html
// http://www.physicsclassroom.com/class/waves/Lesson-0/Motion-of-a-Mass-on-a-Spring

// applies a force to a particle
// TODO perfect forwarding
Particle &operator<<(Particle &lhs, Force &&force)
{
  force /= lhs.mass;          // a = f/m (from f = m*a)
  lhs.acceleration += force;  // add the force to the object's acceleration
  return lhs;
}

// calculates a force between two particles using the function ff
// when given the same particle twice, it returns the identity
Force calc_force(
    const Particle &p1, const Particle &p2,
    std::function<Force(const Particle &p1, const Particle &p2)> ff)
{
  // if (&p1 == &p2) //doesn't work for some reason
  if (p1.location == p2.location)
    {
      //      std::cout << "skipping same particle" << std::endl;
      return glm::vec3(0, 0, 0);
    }
  return ff(p1, p2);
}

// uses calc_force to calculate gravitational force between two particles
Force gravity(const Particle &p1, const Particle &p2,
              std::initializer_list<float> params)
{
  Force result =
      (calc_force(p1, p2, [p1, p2](const Particle &, const Particle &) {
        glm::vec3 direction = glm::normalize(p2.location - p1.location);
//        glm::vec3 direction = p2.location - p1.location;
        float gforce = (p1.mass * p2.mass) / pow(glm::length(direction), 2);
        glm::normalize(direction);

        return (gforce * direction);
      }));
  // for gravity, only gravity constant
  for (auto c : params)
    {
      result *= c;
    }
  return result;
}

Force spring(const Particle &p1, const Particle &p2,
             std::initializer_list<float> params)
{
  Force result =
      calc_force(p1, p2, [p1, p2, params](const Particle &, const Particle &) {
        // f = k*x (k is constant and x
        Spring s(params);  // use params to create spring
        std::cout << s.length << "<- length , const ->" << s.constant
                  << std::endl;
        std::cout << "p2: " << p2 << "p1: " << p1 << std::endl;
        glm::vec3 direction = p2.location - p1.location;
        std::cout << "displacement: " << (s.length - glm::length(direction));
        float magnitude = -1 * s.constant * (s.length - glm::length(direction));
        glm::normalize(direction);
        std::cout << "direction: " << direction << ", magnitude: " << magnitude
                  << std::endl;

        return direction * magnitude;
      });
}

// calc_force for p with each other p in ps and reduce (addition) to one force
// disadvantage: for each pair of particles, the force is calculated twice
Force accumulate(const Particle &p, const ParticleContainer &ps,
                 std::function<Force(const Particle &, const Particle &)> ff)
{
  Force result = glm::vec3{0.0f, 0.0f, 0.0f};
  // std::for_each(std::execution::par, ps.begin(), ps.end(), [p, ff](Particle &
  // n){
  std::for_each(ps.begin(), ps.end(), [p, ff, &result](const Particle &n) {
    result += calc_force(p, n, ff);
  });
  return result;
}

// use a pre-defined function to calculate the forces, pass initializer list for
// parameters
Force accumulate(const Particle &p, const ParticleContainer &ps,
                 std::initializer_list<float> params,
                 std::function<Force(const Particle &, const Particle &,
                                     std::initializer_list<float>)>
                     ff)
{
  Force result = glm::vec3{0.0f, 0.0f, 0.0f};
  // std::for_each(std::execution::par, ps.begin(), ps.end(), [p, ff](Particle &
  // n){
  std::for_each(ps.begin(), ps.end(), [p, params, ff,
                                       &result](const Particle &n) {
    result += ff(
        p, n,
        params);  // don't need to call calc_force on this because ff  does this
  });
  return result;
}

Force accumulate(std::initializer_list<Force> forces)
{
  return std::accumulate(forces.begin(), forces.end(),
                         glm::vec3{0.0f, 0.0f, 0.0f},
                         [](Force a, Force b) {
                           return a + b;
                         });  // TODO try plus, and reduce (C++ 17)
}

// use a force matrix to avoid calculating each force twice (see ForceMatrix for
// more info on how this is done)
//  Force accumulate(Particle p, ForceMatrix & fm)
}
