#include "Particle.cpp"
#include <vector>
#include <array>
#include <iostream>

namespace c3p {

class ParticleSystem
{
  public:
//    using  = std::vector<Particle>;
    using self_t = ParticleSystem;
    using vec = glm::vec3; //std::array<float, 3>;

    ParticleSystem() = delete;

    ParticleSystem(size_t size)
    {
        std::cout << "ParticleSystem constructor called" << std::endl;
        //_particles = new std::vector<Particle>(size, Particle());
        _particles = new std::vector<Particle>();
        for(int i = 0; i<size; ++i)
        {
            _particles->push_back(Particle());
        }
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

    Particle & operator[](int index) const
    {
        return (*_particles)[index];
    }

    Particle & operator[](int index) 
    {
        return (*_particles)[index];
    }


    //TODO pass a view of particles to apply this to
    //TODO perfect forwarding? Or maybe not since I need the force for each vector --> there has to be a better way to do this!
    void applyForceAll(vec force) const
    {
        for(Particle & p : *_particles)
        {
            p.applyForce(force);
        }
    }

    //TODO generalize so that gravitate can also implement gravitateOrigin. Maybe with a range?
    void gravitate(vec point, double strength)
    {
        for(Particle & p : *_particles)
        {
            vec dir = point - p.location();
            glm::normalize(dir);
            dir *= strength;
            p.applyForce(dir);
        }
    }

    //enable origin force --> particles are attracted to their origin
    void gravitateOrigin(float strength) const
    {
        for(Particle & p : *_particles)
        {
            vec dir = p.origin() - p.location();
            glm::normalize(dir);
            dir *= strength;
            p.applyForce(dir);
        }
    }

    void print() const
    {
        for (auto p : *_particles)
        {
            std::cout << "print" << std::endl;
            for(int i = 0; i<3; ++i)
            {
                std::cout << "a " << i << " " << p.acceleration()[i] << std::endl;
    //            std::cout << "v " << i << " " << _velocity[i] << std::endl;
    //            std::cout << "s " << i << " " << _location[i] << std::endl;
            }
        }
    }

    void draw(glm::mat4 & mvp, GLuint MatrixID) const
    {
        for (Particle & p : *_particles)
        {
            p.draw(mvp, MatrixID);
        }
    }

    size_t size() const
    {
        return _particles->size();
    }

    bool empty() const
    {
        return (size() == 0);
    }

  private:
    std::vector<Particle> * _particles;

};

} //namespace c3p
