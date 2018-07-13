#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

namespace c3p {

using vec = glm::vec3;

//struct Particle
//{
//    vec _origin;
//    vec _location;
//    vec _velocity;
//    vec _acceleration;
//    float _mass = 1.0;
//    float _ttl;
//}

//need iterators for algorithms
//could actually also be a template for any kind of "newton's" object
class ParticleContainer
{
    using self_t = ParticleContainer;
    using iterator = ParticleContainer::Iterator;

  public:
    class Iterator
    {
      public:

        Iterator() = default;

      private:
        size_t _index;
        ParticleContainer * _container;
    }

    ParticleContainer()
    {
        _elements = new std::vector<Particle>;
    }

    ParticleContainer(size_t size)
    {
        //TODO initialize with constructor!
        _elements = new std::vector<Particle>(size, Particle());
//        std::generate() 
    }

    ParticleContainer(const self_t & other)
    {
        _elements = new std::vector<Particle>(other._elements);
    }

    ParticleContainer(self_t && other)
    {
        _elements = other._elements;
        other._elements = nullptr;
    }

    ~ParticleContainer()
    {
        delete[] _elements;
    }

    self_t operator=(const self_t & rhs)
    {
        _elements.resize(rhs.size());
        std::copy(rhs.begin(), rhs.end(), (*this).begin);
    }

    self_t operator=(self_t && rhs)
    {
        delete[] _elements;
        _elements = rhs._elements;
        rhs._elements = nullptr;
    }

    bool operator==(self_t & rhs)
    {
        return (this == &rhs || 
                _elements == rhs._elements || 
                std::equal(_elements->begin(), _elements->end(), rhs.begin()));
    }



  private:
    std::vector<Particle> * _elements;
};

} //namespace c3p
