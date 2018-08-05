#include <c3p/newtonian_objects.h>

#include <cmath>
#include <ctime>
#include <iostream>

namespace c3p
{
//  inline float Mass(float mass) {return mass};
//  inline glm::vec3 Location(glm::vec3 loc) {return loc};

std::ostream &operator<<(std::ostream &os, const Particle &p)
{
  os << "origin: (" << p.origin.x << ", " << p.origin.y << ", " << p.origin.z
     << ")\n";
  os << "location: (" << p.location.x << ", " << p.location.y << ", "
     << p.location.z << ")\n";
  os << "velocity: (" << p.velocity.x << ", " << p.velocity.y << ", "
     << p.velocity.z << ")\n";
  os << "acceleration: (" << p.acceleration.x << ", " << p.acceleration.y
     << ", " << p.acceleration.z << ")\n";
  os << "mass: " << p.mass << "\n";
  return os;
}

Particle & randomize(Particle & p)
{
//  std::srand(std::time(nullptr));
  float r; 
  for (int i = 0; i < 3; ++i)
    {
      r = rand();
      r = r / (float)RAND_MAX;
      p.color[i] = r;
      p.location[i] = r * 60 - 30;
    }
    r = rand(); 
    r /= (float)RAND_MAX;
    p.mass = r * 10 + 50;
    p.size = r;

    p.origin = p.location;
    p.velocity = glm::normalize(
                     glm::cross(glm::vec3{p.location[0],
                     p.location[1], 0.0},
                                glm::vec3{0,0,1})) * 0.2f;
  //          p.velocity = glm::normalize(glm::vec3{p.location[0],
  //          p.location[1], 0.0}) * -1.0f;
  return p;
}

//template<class InputIt>
//bool std::find(InputIt first, InputIt last, const Particle & p)
//{
//
//}

bool operator==(const Particle & p, const Particle & other)
{
  return (&p == &other || (p.origin == other.origin && p.mass == other.mass));
}

using Force = glm::vec3;

// inline glm::vec3 force(glm::vec3 f) { return f; }

std::ostream &operator<<(std::ostream &os, const Force &f)
{
  os << "(" << f.x << ", " << f.y << ", " << f.z << ")";
}
}
