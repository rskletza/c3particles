#include <c3p/particle_functions.h>

//#include <execution> c++17
#include <algorithm>
#include <numeric>
#include <iostream>

namespace c3p
{
  glm::vec3 force(glm::vec3 force)
  {
    return force;
  }
  
  std::ostream &operator<<(std::ostream &os, const Force &f)
  {
    os << "(" << f.x << ", " << f.y << ", " << f.z << ")\n";
  }

  //applies a force to a particle
  //TODO perfect forwarding
  Particle & operator<<(Particle & lhs, Force && force)
  {
    std::cout << "force on application: " << force << std::endl;
    force /= lhs.mass;          // a = f/m (from f = m*a)
    std::cout << "force/mass on application: " << force << std::endl;
    lhs.acceleration += force;  // add the force to the object's acceleration
    return lhs;
  }
  
  //calculates a force between two particles using the function ff
  //when given the same particle twice, it returns the identity 
  Force calc_force(const Particle & p1, const Particle & p2, std::function<Force(const Particle &, const Particle &)> ff)
  {
    if (&p1 == &p2) 
    {
      std::cout << "skipping same particle" << std::endl;
      return glm::vec3(0,0,0);
    }
    std::cout << "calc_force" << std::endl;
    return ff(p1, p2);
  }

  //uses calc_force to calculate gravitational force between two particles
  Force gravity(const Particle & p1, const Particle & p2, float G)
  {
    return (G * calc_force(p1, p2, [p1, p2](const Particle &, const Particle &)
    {
      glm::vec3 direction = glm::normalize(p2.location - p1.location);
      float gforce = (p1.mass * p2.mass) / pow(glm::length(direction), 2);

      return (gforce * direction);
    }));
  }

  //calc_force for p with each other p in ps and reduce (addition) to one force
  //disadvantage: for each pair of particles, the force is calculated twice
  Force accumulate(const Particle & p, const ParticleContainer & ps, std::function<Force(const Particle &, const Particle &)> ff)
  {
    Force result = glm::vec3{0.0f, 0.0f, 0.0f};
    //std::for_each(std::execution::par, ps.begin(), ps.end(), [p, ff](Particle & n){
    std::for_each(ps.begin(), ps.end(), [p, ff, &result](const Particle & n){
        result += calc_force(p,n,ff);
        std::cout << "result (accumulate): " << result << std::endl;
        });
  }

  Force accumulate(std::initializer_list<Force> forces)
  {
    return std::accumulate(forces.begin(), forces.end(), glm::vec3{0.0f, 0.0f, 0.0f}, [](Force a, Force b)
    {
      return a + b;
    }); //TODO try plus, and reduce (C++ 17)
  }

  //use a force matrix to avoid calculating each force twice (see ForceMatrix for more info on how this is done)
//  Force accumulate(Particle p, ForceMatrix & fm)

}
