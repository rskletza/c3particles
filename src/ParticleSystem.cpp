#include "Particle.cpp"

namespace c3p {

class ParticleSystem
{
  public:
//    using  = std::vector<Particle>;
    using self_t = ParticleSystem;
    using vec = std::array<float, 3>;

    ParticleSystem() = delete;

    ParticleSystem(size_t size)
    {
        _particles = new std::vector<Particle>(size, Particle());
    }

    ~ParticleSystem()
    {
        delete _particles;
    }

    self_t operator=(const self_t & other)
    {
        if (!empty())
        {
            delete _particles;
        }
        _particles = new std::vector<Particle>(other.size());

    }

    self_t operator=(self_t && other)
    {
        delete _particles;
        _particles = other._particles;
    }

    //TODO perfect forwarding? Or maybe not since I need the force for each vector --> there has to be a better way to do this!
    void applyForce(vec force)
    {
        for(auto p : *_particles)
        {
            p.applyForce(vec force);
        }
    }

    size_t size() const
    {
        return _particles->size();
    }

  private:
    std::vector<Particle> * _particles;

}

} //namespace c3p
