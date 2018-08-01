#ifndef C3P__NEWTONIAN_OBJECTS__INCLUDED
#define C3P__NEWTONIAN_OBJECTS__INCLUDED

#include <iostream>

#include <glm/glm.hpp>

namespace c3p
{
  // Particle implements a concept of an object that is subject to forces (e.g gravitation)
  struct Particle
  {
    glm::vec3 color;  // TODO: does this belong here?
    glm::vec3 origin;
    glm::vec3 location;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
    float ttl;
    Particle() = default;
    Particle(float ms) : mass(ms) {};
  };

  struct Spring
  {
    float length; //length of relaxed spring
    float constant; //spring constant
    glm::vec3 end; //spring is fixed at location

    Spring() = delete;
    Spring(float l, float c) : length(l), constant(c) { }
    Spring(float l, float c, glm::vec3 pos) : length(l), constant(c), end(pos) { }
    Spring(std::initializer_list<float> l)
    { 
      auto it = l.begin();
      length = *it;
      constant = *(++it);
    }
  };

  using Force = glm::vec3;
  
//  inline float Mass(float mass) {return mass};
//  inline glm::vec3 Location(glm::vec3 loc) {return loc};

  std::ostream &operator<<(std::ostream &os, const Particle &p);

  //inline glm::vec3 force(glm::vec3 f) { return f; }
  
  std::ostream &operator<<(std::ostream &os, const Force &f);

}

#endif //C3P__NEWTONIAN_OBJECTS__INCLUDED
