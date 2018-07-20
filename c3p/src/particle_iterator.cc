
using iterator_category = std::random_access_iterator_tag;
using value_type = Particle;
using difference_type = std::ptrdiff_t;
using pointer = value_type *;
using reference = value_type &;

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

private:
ParticleContainer *_container;
size_t _index;
}
