
#include <c3p/particle_system.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include <vector>

namespace c3p
{
using ParticleContainer = std::vector<Particle>;
using vec = glm::vec3;
using self_t = ParticleSystem;

ParticleSystem::ParticleSystem(size_t size)
    : _G(10e-4)  // universal gravitational constant (actually 6.67*10^-11)
      ,
      _particles(size),
      _requested_size(size)
{
}

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
  for (Particle &p : _particles)
    {
      randomize(p);
    }
}

void ParticleSystem::reset()
{
  for (Particle &p : _particles)
    {
      p.location = p.origin;
      p.velocity = glm::vec3(0, 0, 0);
      p.acceleration = glm::vec3(0, 0, 0);
    }

  if (_requested_size <= _particles.size())
    {
      _particles.resize(_requested_size);
    }
  else if (_requested_size >= _particles.size())
    {
      while (_requested_size != _particles.size())
        {
          auto p = Particle();
          randomize(p);
          _particles.push_back(std::move(p));
        }
    }
}

//void ParticleSystem::add(Particle &&p) { _particles.push_back(p); }
//void ParticleSystem::remove(Particle &p)
//{
//  _particles.erase(std::find(_particles.begin(), _particles.end(), p));
//}

void ParticleSystem::reverse()
{
  for (Particle &p : _particles)
    {
      p.velocity *= -1.0f;
    }
}

void ParticleSystem::setGexponent(int exp) { _G = 1.0 * std::pow(10, exp); }

void ParticleSystem::requestParticles(size_t size) { _requested_size = size; }

float ParticleSystem::g_constant() const { return _G; }

size_t ParticleSystem::size() const { return _particles.size(); }

bool ParticleSystem::empty() const { return _particles.empty(); }

ParticleContainer::iterator ParticleSystem::begin()
{
  return _particles.begin();
}

ParticleContainer::iterator ParticleSystem::end() { return _particles.end(); }

ParticleContainer &ParticleSystem::particles() { return _particles; }

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
}  // namespace c3p
