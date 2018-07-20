
#include <c3p/particle_system.h>

#include <cmath>
#include <iostream>
#include <vector>

namespace c3p
{
using ParticleContainer = std::vector<Particle>;
using vec = glm::vec3;  // std::array<float, 3>;
using self_t = ParticleSystem;

// Some sites I used to brush up on my physics:
// - http://www.pas.rochester.edu/~blackman/ast104/newton3laws16.html
// -
// https://www.johannes-strommer.com/rechner/basics-mathe-mechanik/ruck-beschleunigung-geschwindigkeit-weg
// -
// http://zonalandeducation.com/mstm/physics/mechanics/forces/newton/mightyFEqMA/mightyFEqMA.html
// -
// http://physics.weber.edu/amiri/physics1010online/WSUonline12w/OnLineCourseMovies/CircularMotion&Gravity/reviewofgravity/ReviewofGravity.html
// -
// http://www.physicsclassroom.com/class/waves/Lesson-0/Motion-of-a-Mass-on-a-Spring

// Apply a force
// Particle implements a concept of an object that is subject to gravitation
void applyForce(glm::vec3 force, Particle &p)
{
  force /= p.mass;          // f = m*a
  p.acceleration += force;  // add the force to the object's acceleration
}

// ParticleSystem::ParticleSystem() = delete;

ParticleSystem::ParticleSystem(size_t size)
    : _G(1.0 *
         std::pow(
             10,
             -4))  // universal gravitational constant (actually 6.67*10^-11)
{
  std::cout << "ParticleSystem constructor called" << std::endl;
  //_particles = new std::vector<Particle>(size, Particle());
  _particles = new std::vector<Particle>();
  for (int i = 0; i < size; ++i)
    {
      _particles->push_back(Particle());
    }
}

ParticleSystem::~ParticleSystem() { delete _particles; }
self_t ParticleSystem::operator=(const self_t &other)
{
  if (!empty())
    {
      delete _particles;
    }
  _particles = new std::vector<Particle>(other.size());
}

self_t ParticleSystem::operator=(self_t &&other)
{
  delete _particles;
  _particles = other._particles;
}

// give each particle a random color and location. the velocity is set
// perpendicular to the location vector (this is what causes the circular
// movement around 0,0,0)
void ParticleSystem::setRandom()
{
  for (Particle &p : *_particles)
    {
      for (int i = 0; i < 3; ++i)
        {
          float r = rand();
          r = r / (float)RAND_MAX;
          p.color[i] = r;
          p.location[i] = r * 60 - 30;
          p.mass = r * 10 + 200;
          //      p.mass = 100;
          //    if(r>0.5) { p.location[1] = 0.0;}
          //    else {p.location[0] = 0.0;}
        }
          p.origin = p.location;
          p.velocity = glm::normalize(glm::cross(
                           glm::vec3{p.location[0], p.location[1], 0.0},
                           glm::vec3{0.0, 0.0, 1.0}))
                        * 0.5f;
//          p.velocity = glm::normalize(glm::vec3{p.location[0], p.location[1], 0.0}) * -1.0f;
          p.location[2] = 0.0;
    }
}

void ParticleSystem::update()
{
  // deltaT will be 1 because calculation is based on frames (and frames are run
  // at 60Hz)
  float deltaT = 1.0;
  for (Particle &p : *_particles)
    {
      // v(t)= Int(acc) = acc*t + C
      // C is the integration constant, which is the velocity at the previous
      // point in time
      //--> v(t) = a*t + v(t-1)
      // t in this case is equal to 1, as I am using frames to measure time
      // (TODO
      // frames are not always constant, use actual time eg seconds with delta
      // glfwGetTime  as a result, the velocity v(t) = acc*1 + v(t-1)
      p.velocity += p.acceleration * deltaT;

      // s(t) = Int(velocity) = Int(acc*t + C) = (a*t^2)/2 + C*t + C1
      // C1 is the location at the previous point in time
      //--> s(t) = (a*t^2)/2 + v(t) + s(t-1)
      // again, t is equal to
      p.location += (p.acceleration * deltaT) / 2.0f + p.velocity;

      p.acceleration = {0.0, 0.0,
                        0.0};  // acceleration is recalculated every time
    }
}

// TODO pass a view of particles to apply this to
// TODO perfect forwarding? Or maybe not since I need the force for each vector
// --> there has to be a better way to do this!
void ParticleSystem::applyForceAll(glm::vec3 force) const
{
  for (Particle &p : *_particles)
    {
      applyForce(force, p);
    }
}

// TODO generalize so that gravitate can also implement gravitateOrigin. Maybe
// with a range?
void ParticleSystem::addAttractor(glm::vec3 point, float strength) const
{
  for (Particle &p : *_particles)
    {
      glm::vec3 force = glm::normalize(point - p.location) * strength;
      applyForce(force, p);
    }
}

// enable origin force --> particles are attracted to their origin
void ParticleSystem::gravitateOrigin(float strength) const
{
  for (Particle &p : *_particles)
    {
      glm::vec3 force = glm::normalize(p.origin - p.location) * strength;
      applyForce(force, p);
    }
}

// simulate gravitational forces on all the particles
void ParticleSystem::nbodyGravity() const
{
  for (Particle &p : *_particles)
    {
      float gforce;
      glm::vec3 direction;
      for (Particle &other : *_particles)
        {
          if (p.location == other.location) continue;
          direction = glm::normalize(other.location - p.location);
          gforce = _G * (p.mass * other.mass) / pow(glm::length(direction), 2);
        }
      applyForce(gforce * direction, p);

      // TODO use addGForce here
    }
}

// stiff spring has high tension (spring constant)
void ParticleSystem::nbodySprings(float tension) const {}
void ParticleSystem::addGForce(glm::vec3 position, float mass) const
{
  for (Particle &p : *_particles)
    {
      glm::vec3 direction = glm::normalize(position - p.location);
      float gforce = _G * (p.mass * mass) / pow(glm::length(direction), 2);

      applyForce(gforce * direction, p);
    }
}

void ParticleSystem::print() const
{
  for (auto p : *_particles)
    {
      std::cout << "print" << std::endl;
      for (int i = 0; i < 3; ++i)
        {
          //            std::cout << "a " << i << " " << p.acceleration[i] <<
          //            std::endl; std::cout << "v " << i << " " <<
          //            p.velocity[i] <<
          //            std::endl;
          std::cout << "s " << i << " " << p.location[i] << std::endl;
        }
    }
}

#if TODO
std::ostream &operator<<(std::ostream &os, const ParticleSystem &ps)
{
  for (const auto &p : ps.particles())
    {
      os << p;
    }
}
#endif

size_t ParticleSystem::size() const { return _particles->size(); }
bool ParticleSystem::empty() const { return _particles->empty(); }
}  // namespace c3p
