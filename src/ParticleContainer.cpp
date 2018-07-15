//#include "ParticleContainer.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include <glm/glm.hpp>

namespace c3p {

using vec = glm::vec3;

//struct Particle
//{
//    vec origin;
//    vec location;
//    vec velocity;
//    vec acceleration;
//    float mass;
//    float ttl;
//}

//need iterators for algorithms
//change name to NewtonianObjectContainer? Urgh
template<class NewtonianObject>
class ParticleContainer
{
    using self_t = ParticleContainer<NewtonianObject>;
//    using value_t = Object;
    using iterator = ParticleContainer<NewtonianObject>::Iterator;

  public:
    class Iterator
    {
      public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = NewtonianObject;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

        Iterator() = default;

        Iterator(ParticleContainer * container, size_t index)
          : _container(container)
          , _index(index)
        {   }

        Iterator(const & iterator other)
          : _container(other._container)
          , _index(other._index)
        {   }

        ~Iterator() = default;

        iterator & operator=(const & iterator other) = default;

        const reference operator*() const
        {
            return _container[_index];
        }

        reference operator*()
        {
            return _container[_index];
        }

        const reference operator[](int offset) const
        {
            return *(*this + offset);
        }

        reference operator[](int offset) 
        {
            return *(*this + offset);
        }

        iterator & operator++() 
        {
            ++_index;
            return *this;
        }

        iterator & operator++(int) 
        {
            iterator old = *this;
            ++(*this);
            return old;
        }

        iterator & operator--() 
        {
            --_index;
            return *this;
        }

        iterator & operator--(int) 
        {
            iterator old = *this;
            --(*this);
            return old;
        }

        iterator & operator+=(int offset)
        {
            _index += offset;
            return *this;
        }

        iterator & operator-=(int offset)
        {
            _index -= offset;
            return *this;
        }

        iterator & operator+(int offset) const
        {
            return (*this)[offset];
        }

        iterator & operator-(int offset) const
        {
            return (*this)[-offset];
        }

        difference_type operator-(const iterator & other)
        {
            return (_index - other._index);
        }

        bool operator==(const iterator & other)
        {
            return (this == &other || 
                    (_container == other._container && _index == other.index));
        }

        bool operator!=(const iterator & other)
        {
            return !(&this == other);
        }

        //we'll see if we need comparisons

      private:
        ParticleContainer * _container;
        size_t _index;
    }

    ParticleContainer()
    {
        _elements = new std::vector<Particle>;
    }

    ParticleContainer(size_t size)
    {
        //TODO initialize with constructor!
        _elements = new std::vector<NewtonianObject>(size, NewtonianObject());
//        std::generate() 
    }

    ParticleContainer(const self_t & other)
    {
        _elements = new std::vector<NewtonianObject>(other._elements);
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

    bool operator==(self_t & rhs) const
    {
        return (this == &rhs || 
                _elements == rhs._elements || 
                std::equal(_elements->begin(), _elements->end(), rhs.begin()));
    }

    bool operator!=(self_t & rhs) const
    {
        return !(*this != rhs);
    }

    bool empty() const
    {
        return _elements->empty();
    }

    bool size() const
    {
        return _elements->size();
    }

    iterator begin() const
    {
        return iterator(this, 0);
    }

    iterator end() const
    {
        return iterator(this, size());
    }

    template<class T>
    void add(T && object)
    {
        _elements->push_back(std::forward<T>(object));
    }

    iterator add(size_t count)
    {
        for(;count!=0;--count)
        {
            _elements->emplace(this->end())
        }
        return iterator(this, size())
    }

    iterator remove(iterator & it)
    {
        _elements->erase(it);
        return it; //because this is a contiguous container it now points to the next element
    }

  private:
    std::vector<NewtonianObject> * _elements;
};

} //namespace c3p
