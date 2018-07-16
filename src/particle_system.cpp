//#include "ParticleSystem.h"

#include <vector>
#include <iostream>
#include <cmath>

#include "src/particle_system.h"

namespace c3p {
    
//some sites I used to brush up on my physics knowledge:
//http://www.pas.rochester.edu/~blackman/ast104/newton3laws16.html
//https://www.johannes-strommer.com/rechner/basics-mathe-mechanik/ruck-beschleunigung-geschwindigkeit-weg
//http://zonalandeducation.com/mstm/physics/mechanics/forces/newton/mightyFEqMA/mightyFEqMA.html
//http://physics.weber.edu/amiri/physics1010online/WSUonline12w/OnLineCourseMovies/CircularMotion&Gravity/reviewofgravity/ReviewofGravity.html
//http://www.physicsclassroom.com/class/waves/Lesson-0/Motion-of-a-Mass-on-a-Spring

//apply a force
//concept von etwas, worauf gravitation wirken kann
void applyForce(glm::vec3 force, Particle & p)
{
    force /= p.mass; //f = m*a
    p.acceleration += force;
}

class ParticleSystem
{
    using ParticleContainer = std::vector<Particle>;
    using vec = glm::vec3; //std::array<float, 3>;
    using self_t = ParticleSystem;

    friend ParticleRenderer; //TODO find something better than friend

  public:

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

//    Particle & operator[](int index) const
//    {
//        return (*_particles)[index];
//    }
//
//    Particle & operator[](int index) 
//    {
//        return (*_particles)[index];
//    }

    const ParticleContainer & readParticles() const
    {
        return *(_particles);
    }

    //give each particle a random color and location. the velocity is set perpendicular to the location vector (this is what causes the circular movement around 0,0,0)
    void setRandom()
    {
        for(Particle & p : *_particles)
        {
            for (int i = 0; i < 3; ++i)
            {
                float r = rand();
                r = r/ (float) RAND_MAX;
                p.color[i] = r;
                p.location[i] = r*40-20;
                p.origin = p.location;
                p.velocity = glm::normalize(glm::cross(vec{p.location[0], p.location[1], 0.0}, vec{0.0,0.0,1.0}))*0.5f;
                p.mass = r*10 + 200;
            }
            p.location[2] = 0.0;
        }
    }

    void update() 
    {
        //deltaT will be 1 because calculation is based on frames (and frames are run at 60Hz)
        float deltaT = 1.0;
        for(Particle & p : *_particles)
        {
            //v(t)= Int(acc) = acc*t + C
            //C is the integration constant, which is the velocity at the previous point in time
            //--> v(t) = a*t + v(t-1)
            //t in this case is equal to 1, as I am using frames to measure time (TODO frames are not always constant, use actual time eg seconds with delta glfwGetTime
            //as a result, the velocity v(t) = acc*1 + v(t-1)
            p.velocity += p.acceleration*deltaT; 

            //s(t) = Int(velocity) = Int(acc*t + C) = (a*t^2)/2 + C*t + C1
            //C1 is the location at the previous point in time
            //--> s(t) = (a*t^2)/2 + v(t) + s(t-1)
            //again, t is equal to 
            p.location += (p.acceleration*deltaT)/2.0f + p.velocity;

            p.acceleration = {0.0,0.0,0.0}; //acceleration is recalculated every time
        }
    }
    

    //TODO pass a view of particles to apply this to
    //TODO perfect forwarding? Or maybe not since I need the force for each vector --> there has to be a better way to do this!
    void applyForceAll(vec force) const
    {
        for(Particle & p : *_particles)
        {
            applyForce(force, p);
        }
    }

    //TODO generalize so that gravitate can also implement gravitateOrigin. Maybe with a range?
    void addAttractor(vec point, double strength)
    {
        for(Particle & p : *_particles)
        {
            vec force = glm::normalize(point - p.location) * strength;
            applyForce(force, p);
        }
    }

    //enable origin force --> particles are attracted to their origin
    void gravitateOrigin(float strength) const
    {
        for(Particle & p : *_particles)
        {
            vec force = glm::normalize(p.origin - p.location) * strength;
            applyForce(force, p);
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
                if(p.location == other.location)
                    continue;
                direction = glm::normalize(other.location-p.location);
                gforce = _G * (p.mass * other.mass) / pow(glm::length(direction), 2);
            }
            applyForce(gforce * direction, p);

            //TODO use addGForce here
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
            vec direction = glm::normalize(position - p.location);
            float gforce = _G*(p.mass * mass)/pow(glm::length(direction),2);

            applyForce(gforce * direction, p);
        }
    }

    void print() const
    {
        for (auto p : *_particles)
        {
            std::cout << "print" << std::endl;
            for(int i = 0; i<3; ++i)
            {
    //            std::cout << "a " << i << " " << p.acceleration[i] << std::endl;
    //            std::cout << "v " << i << " " << p.velocity[i] << std::endl;
                std::cout << "s " << i << " " << p.location[i] << std::endl;
            }
        }
    }

//    void draw(glm::mat4 & mvp, GLuint MatrixID) const
//    {
//        for (Particle & p : *_particles)
//        {
//            p.draw(mvp, MatrixID);
//        }
//    }

    size_t size() const
    {
        return _particles->size();
    }

    bool empty() const
    {
        return (size() == 0);
    }

  private:
    ParticleContainer * _particles;
    float _G;

};

} //namespace c3p
