//#include "ParticleSystem.h"

#include <vector>
#include <array>
#include <iostream>
#include <cmath>

#include "Particle.cpp"

namespace c3p {

//some sites I used to brush up on my physics knowledge:
//http://www.pas.rochester.edu/~blackman/ast104/newton3laws16.html
//https://www.johannes-strommer.com/rechner/basics-mathe-mechanik/ruck-beschleunigung-geschwindigkeit-weg/#beispiel
//http://zonalandeducation.com/mstm/physics/mechanics/forces/newton/mightyFEqMA/mightyFEqMA.html
//http://physics.weber.edu/amiri/physics1010online/WSUonline12w/OnLineCourseMovies/CircularMotion&Gravity/reviewofgravity/ReviewofGravity.html
//http://www.physicsclassroom.com/class/waves/Lesson-0/Motion-of-a-Mass-on-a-Spring

class ParticleSystem
{
  public:
//    using  = std::vector<Particle>;
    using self_t = ParticleSystem;
    using vec = glm::vec3; //std::array<float, 3>;

    ParticleSystem() = delete;

    ParticleSystem(size_t size)
        :_G(1.0*std::pow(10,-2)) //universal gravitational constant (actually 6.67*10^-11)
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
    void addAttractor(vec point, double strength)
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
            vec dir = glm::normalize(p.origin() - p.location());
            dir *= strength;
            p.applyForce(dir);
        }
    }

    //simulate gravitational forces on all the particles
    void nbodyGravity() const
    {
        for(Particle & p : *_particles)
        {
            float gforce;
            vec direction;
            for(Particle & other : *_particles)
            {
                if(p.location() == other.location())
                    continue;
                direction = other.location()-p.location();
                gforce = _G*(p.mass()*other.mass())/pow(glm::length(direction),2);
            }
            p.applyForce(gforce*(glm::normalize(direction)));
        }
    }

    //stiff spring has high tension (spring constant)
    void nbodySprings(float tension) 
    {

    }

    void addGForce(vec position, float mass)
    {
        for(Particle & p : *_particles)
        {
            vec direction = position - p.location();
            float gforce = _G*(p.mass()*mass)/pow(glm::length(direction),2);

            p.applyForce(gforce*glm::normalize(direction));
        }
    }

    void print() const
    {
        for (auto p : *_particles)
        {
            std::cout << "print" << std::endl;
            for(int i = 0; i<3; ++i)
            {
    //            std::cout << "a " << i << " " << p.acceleration()[i] << std::endl;
    //            std::cout << "v " << i << " " << p.velocity()[i] << std::endl;
                std::cout << "s " << i << " " << p.location()[i] << std::endl;
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
    float _G;

};

} //namespace c3p
