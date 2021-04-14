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

ParticleContainer &operator<<(ParticleContainer &lhs, ForceMatrix &rhs)
{
  for (size_t y = 0; y < lhs.size(); ++y)
    {
      lhs[y] << accumulate(
          rhs[y]);  // rhs[y] returns a vector of all forces for particle lhs[b]
    }
  return lhs;
}

// calculates a force between two particles using the function ff
// when given the same particle twice, it returns the identity
Force calc_force(
    const Particle &p1, const Particle &p2,
    std::function<Force(const Particle &p1, const Particle &p2)> ff)
{
  if (p1.location == p2.location)
    {
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
        glm::vec3 direction = p2.location - p1.location;
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
        // f = -k*x (k is spring constant, x is stretch or compression)
        Spring s(params);  // use params to create spring
        glm::vec3 direction = p2.location - p1.location;
        float magnitude = -1 * s.constant * (s.length - glm::length(direction));
        glm::normalize(direction);

        return direction * magnitude;
      });
  return result;
}

Force simple_attract(const Particle &p1, const Particle &p2,
                     std::initializer_list<float> params)
{
  Force result =
      calc_force(p1, p2, [p1, p2](const Particle &, const Particle &) {
        glm::vec3 direction = glm::normalize(p2.location - p1.location);
        float force = (p1.mass * p2.mass) / pow(glm::length(direction), 2);
        return (force * direction);
      });
  for (auto c : params)
    {
      result *= c;
    }
  return result;
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

Force accumulate(std::vector<Force> forces)
{
  return std::accumulate(forces.begin(), forces.end(),
                         glm::vec3{0.0f, 0.0f, 0.0f},
                         [](Force a, Force b) {
                           return a + b;
                         });  // TODO try plus, and reduce (C++ 17)
}

void update(Particle &p)
{
  // deltaT will be 1 because calculation is based on frames
  // v(t)= Int(acc) = acc*t + C
  // C is the integration constant, which is the velocity at the previous
  // point in time
  //--> v(t) = a*t + v(t-1)
  // t in this case is equal to 1
  // as a result, the velocity v(t) = acc*1 + v(t-1)
  p.velocity += p.acceleration * 1.0f;  // deltaT = 1.0

  // s(t) = Int(velocity) = Int(acc*t + C) = (a*t^2)/2 + C*t + C1
  // C1 is the location at the previous point in time
  //--> s(t) = (a*t^2)/2 + v(t) + s(t-1)
  // again, t is equal to 1
  p.location += (p.acceleration * 1.0f) / 2.0f + p.velocity;  // deltaT^2 = 1.0

  p.acceleration = {0, 0, 0};  // acceleration is recalculated every time
}

}  // namespace c3p
