#include <c3p/particle_container.h>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include <glm/glm.hpp>

#include <c3p/particle_system.h>  //needed for struct Particle

namespace c3p
{
using vec = glm::vec3;
using self_t = ParticleContainer;
// using iterator = ParticleContainer::iterator;

using iterator_category = std::random_access_iterator_tag;
using value_type = Particle;
using difference_type = std::ptrdiff_t;
using pointer = value_type *;
using reference = value_type &;
using iterator_t = ParticleContainer::iterator;

ParticleContainer::iterator::iterator(ParticleContainer *container,
                                      size_t index)
    : _container(container), _index(index)
{
}

ParticleContainer::iterator::iterator(const &iterator other)
    : _container(other._container), _index(other._index)
{
}

iterator_t &operator=(const &iterator_t other)
{
  this->_container = other._container;
  this->_index = other._index;
  return *this;
}

const reference ParticleContainer::iterator::operator*() const
{
  return _container[_index];
}

reference ParticleContainer::iterator::operator*()
{
  return _container[_index];
}

const reference ParticleContainer::iterator::operator[](int offset) const
{
  return *(*this + offset);
}

reference ParticleContainer::iterator::operator[](int offset)
{
  return *(*this + offset);
}

iterator ParticleContainer::iterator::&operator++()
{
  ++_index;
  return *this;
}

iterator ParticleContainer::iterator::&operator++(int)
{
  iterator old = *this;
  ++(*this);
  return old;
}

iterator &ParticleContainer::iterator::operator--()
{
  --_index;
  return *this;
}

iterator &ParticleContainer::iterator::operator--(int)
{
  iterator old = *this;
  --(*this);
  return old;
}

iterator &ParticleContainer::iterator::operator+=(int offset)
{
  _index += offset;
  return *this;
}

iterator &ParticleContainer::iterator::operator-=(int offset)
{
  _index -= offset;
  return *this;
}

iterator &ParticleContainer::iterator::operator+(int offset) const
{
  return (*this)[offset];
}

iterator &ParticleContainer::iterator::operator-(int offset) const
{
  return (*this)[-offset];
}

difference_type ParticleContainer::iterator::operator-(const iterator &other)
{
  return (_index - other._index);
}

bool ParticleContainer::iterator::operator==(const iterator &other) const
{
  return (this == &other ||
          (_container == other._container && _index == other.index));
}

bool ParticleContainer::iterator::operator!=(const iterator &other) const
{
  return !(&this == other);
}
// we'll see if we need comparisons

ParticleContainer::ParticleContainer()
{
  _elements = new std::vector<Particle>;
}

ParticleContainer::ParticleContainer(size_t size)
{
  // TODO initialize with constructor!
  _elements = new std::vector<Particle>(size, Particle());
  //        std::generate()
}

ParticleContainer::ParticleContainer(const self_t &other)
{
  _elements = new std::vector<Particle>(*(other._elements));
}

ParticleContainer::ParticleContainer(self_t &&other)
{
  _elements = other._elements;
  other._elements = nullptr;
}

ParticleContainer::~ParticleContainer() { delete[] _elements; }
self_t operator=(const self_t &rhs)
{
  _elements.resize(rhs.size());
  std::copy(rhs.begin(), rhs.end(), (*this).begin);
}

self_t ParticleContainer::operator=(self_t &&rhs)
{
  delete[] _elements;
  _elements = rhs._elements;
  rhs._elements = nullptr;
}

bool ParticleContainer::operator==(self_t &rhs) const
{
  return (this == &rhs || _elements == rhs._elements ||
          std::equal(_elements->begin(), _elements->end(), rhs.begin()));
}

bool ParticleContainer::operator!=(self_t &rhs) const
{
  return !(*this != rhs);
}

bool ParticleContainer::empty() const { return _elements->empty(); }
bool ParticleContainer::size() const { return _elements->size(); }
iterator ParticleContainer::begin() const { return iterator(this, 0) }
iterator ParticleContainer::end() const { return iterator(this, size()) }
void ParticleContainer::add(Particle &&object)
{
  _elements->push_back(std::forward<T>(object));
}

iterator ParticleContainer::add(size_t count)
{
  for (; count != 0; --count)
    {
      _elements->emplace(this->end())
    }
  return end();
}

iterator ParticleContainer::remove(iterator &it)
{
  _elements->erase(it);
  return it;  // because this is a contiguous container it now points to the
}

}  // namespace c3p
