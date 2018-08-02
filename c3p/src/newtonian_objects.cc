#include <c3p/newtonian_objects.h>

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

using Force = glm::vec3;

// inline glm::vec3 force(glm::vec3 f) { return f; }

std::ostream &operator<<(std::ostream &os, const Force &f)
{
  os << "(" << f.x << ", " << f.y << ", " << f.z << ")\n";
}
}
