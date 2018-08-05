
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

// ParticleSystem::ParticleSystem() = delete;

//TODO std::generate with randomize
ParticleSystem::ParticleSystem(size_t size)
    : _G(10e-4)  // universal gravitational constant (actually 6.67*10^-11)
    , _particles(size)
    , _requested_size(size)
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
  _requested_size = other._requested_size;
}

// give each particle a random color and location. the velocity is set
// perpendicular to the location vector (this is what causes the circular
// movement around 0,0,0)
void ParticleSystem::setRandom()
{
  for (Particle &p : _particles) { randomize(p); }
}

void ParticleSystem::reset()
{
  for (Particle &p : _particles)
    {
      p.location = p.origin;
      p.velocity = glm::vec3(0,0,0);
      p.acceleration = glm::vec3(0,0,0);
//      p.velocity = glm::normalize(
//                       glm::cross(glm::vec3{p.location[0],
//                       p.location[1], 0.0},
//                                  glm::vec3{0.0, 0.0, 1.0})) *
//                   0.2f;
    }

  if(_requested_size <=_particles.size())
  {
    std::cout << "delete particles" << std::endl;
    _particles.resize(_requested_size);
  }
  else if(_requested_size >= _particles.size())
  {
    std::cout << "add particles" << std::endl;
    while (_requested_size != _particles.size())
    {
      auto p = Particle();
      randomize(p);
      _particles.push_back(std::move(p));
    }
  }

}

void ParticleSystem::add(Particle && p)
{
  _particles.push_back(p);
}

void ParticleSystem::remove(Particle & p)
{
  _particles.erase(std::find(_particles.begin(), _particles.end(), p));
}

//void Particle::remove(size_t n)
//{
//  _particles.erase(_particles.back())
//}

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

void ParticleSystem::setGexponent(int exp) { _G = 1.0 * std::pow(10, exp); }

void ParticleSystem::requestParticles(size_t size) { _requested_size = size; }

float ParticleSystem::g_constant() const { return _G; }

ParticleContainer::iterator ParticleSystem::begin() { return _particles.begin(); }

ParticleContainer::iterator ParticleSystem::end() { return _particles.end(); }

ParticleContainer &ParticleSystem::particles()
{
  return _particles;
}

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
  return os;
}

size_t ParticleSystem::size() const { return _particles.size(); }
bool ParticleSystem::empty() const { return _particles.empty(); }
}  // namespace c3p
