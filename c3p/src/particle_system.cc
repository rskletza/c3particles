
#include <c3p/particle_system.h>

#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>

namespace c3p
{
using ParticleContainer = std::vector<Particle>;
using vec = glm::vec3;  // std::array<float, 3>;
using self_t = ParticleSystem;

// Apply a force
// Particle implements a concept of an object that is subject to gravitation
// void applyForce(glm::vec3 force, Particle &p)
//{
//  force /= p.mass;          // f = m*a
//  p.acceleration += force;  // add the force to the object's acceleration
//}

// ParticleSystem::ParticleSystem() = delete;

ParticleSystem::ParticleSystem(size_t size)
    : _G(10e-4)  // universal gravitational constant (actually 6.67*10^-11)
    , _particles(size)
{ }

ParticleSystem::~ParticleSystem() = default;  // { delete _particles; }

self_t ParticleSystem::operator=(const self_t &other)
{
  _particles = other._particles;
  _G = other._G;
}

self_t ParticleSystem::operator=(self_t &&other)
{
  _particles = other._particles;
  _G = other._G;
}

// give each particle a random color and location. the velocity is set
// perpendicular to the location vector (this is what causes the circular
// movement around 0,0,0)
void ParticleSystem::setRandom()
{
  //  std::srand(std::time(nullptr));
  for (Particle &p : _particles)
    {
      for (int i = 0; i < 3; ++i)
        {
          float r = rand();
          r = r / (float)RAND_MAX;
          p.color[i] = r;
          p.location[i] = r * 60 - 30;
          p.mass = r * 10 + 100;
          p.size = r;
        }
      //      p.mass = 10;
      p.location[2] = 0.0;
      std::cout << p.size << std::endl;
      p.origin = p.location;
      p.velocity = glm::normalize(
                       glm::cross(glm::vec3{p.location[0],
                       p.location[1], 0.0},
                                  glm::vec3{0.0, 0.0, 1.0})) *
                   0.2f;
      //          p.velocity = glm::normalize(glm::vec3{p.location[0],
      //          p.location[1], 0.0}) * -1.0f;
    }
}

void ParticleSystem::reset()
{
  for (Particle &p : _particles)
    {
      p.location = p.origin;
//      p.location[2] = 0.0;
      p.velocity *= 0;
      p.velocity = glm::normalize(
                       glm::cross(glm::vec3{p.location[0],
                       p.location[1], 0.0},
                                  glm::vec3{0.0, 0.0, 1.0})) *
                   0.2f;
    }
}

void ParticleSystem::reverse()
{
  for (Particle &p : _particles)
    {
      p.velocity *= -1.0f;
    }
}

void ParticleSystem::update()
{
  // deltaT will be 1 because calculation is based on frames 
  for (Particle &p : _particles)
//  std::transform(_particles.begin(), _particles.end(), _particles.begin(), [](Particle & p)
    {
      // v(t)= Int(acc) = acc*t + C
      // C is the integration constant, which is the velocity at the previous
      // point in time
      //--> v(t) = a*t + v(t-1)
      // t in this case is equal to 1
      // as a result, the velocity v(t) = acc*1 + v(t-1)
      p.velocity += p.acceleration * 1.0f; //deltaT = 1.0

      // s(t) = Int(velocity) = Int(acc*t + C) = (a*t^2)/2 + C*t + C1
      // C1 is the location at the previous point in time
      //--> s(t) = (a*t^2)/2 + v(t) + s(t-1)
      // again, t is equal to 1
      p.location += (p.acceleration * 1.0f) / 2.0f + p.velocity; //deltaT^2 = 1.0

      p.acceleration = {0, 0, 0};  // acceleration is recalculated every time
    }
//    );
}

// void ParticleSystem::applyForceAll(glm::vec3 force)
//{
//  for (Particle &p : _particles)
//    {
//      applyForce(force, p);
//    }
//}
//
// void ParticleSystem::addAttractor(glm::vec3 point, float strength)
//{
//  for (Particle &p : _particles)
//    {
//      glm::vec3 force = glm::normalize(point - p.location) * strength;
//      applyForce(force, p);
//    }
//}
//
//// enable origin force --> particles are attracted to their origin
// void ParticleSystem::gravitateOrigin(float strength)
//{
//  for (Particle &p : _particles)
//    {
//      glm::vec3 force = glm::normalize(p.origin - p.location) * strength;
//      applyForce(force, p);
//    }
//}
//
//// simulate gravitational forces on all the particles
// void ParticleSystem::nbodyGravity()
//{
//  for (Particle &p : _particles)
//    {
//      float gforce;
//      glm::vec3 direction;
//      for (Particle &other : _particles)
//        {
//          if (p.location == other.location) continue;
//          direction = glm::normalize(other.location - p.location);
//          gforce = _G * (p.mass * other.mass) / pow(glm::length(direction),
//          2);
//        }
//      applyForce(gforce * direction, p);
//    }
//}
//
// void ParticleSystem::addGForce(glm::vec3 position, float mass)
//{
//  for (Particle &p : _particles)
//    {
//      glm::vec3 direction = glm::normalize(position - p.location);
//      float gforce = _G * (p.mass * mass) / pow(glm::length(direction), 2);
//
//      applyForce(gforce * direction, p);
//    }
//}

void ParticleSystem::setGexponent(int exp) { _G = 1.0 * std::pow(10, exp); }

float ParticleSystem::g_constant() const { return _G; }

ParticleContainer::iterator ParticleSystem::begin() { return _particles.begin(); }

ParticleContainer::iterator ParticleSystem::end() { return _particles.end(); }

const ParticleContainer &ParticleSystem::particles() const
{
  return _particles;
}

std::ostream &operator<<(std::ostream &os, const ParticleSystem &ps)
{
  for (const Particle &p : ps.particles())
    {
      os << p;
    }
}

size_t ParticleSystem::size() const { return _particles.size(); }
bool ParticleSystem::empty() const { return _particles.empty(); }
}  // namespace c3p
