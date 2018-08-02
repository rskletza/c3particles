#ifndef C3P__PARTICLE_CONTAINER__INCLUDED
#define C3P__PARTICLE_CONTAINER__INCLUDED

#include <iterator>
#include <vector>

#include <glm/glm.hpp>

namespace c3p
{
// forward declarations
struct Particle;
class ParticleContainer;

using vec = glm::vec3;

class ParticleContainer
{
  //  using iterator = std::iterator<std::random_access_iterator_tag, Particle,
  //  std::ptrdiff_t, Particle*, Particle&>;

 public:
  class iterator
  {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = Particle;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;
    using iterator_t = ParticleContainer::iterator;

    iterator() = default;

    iterator(ParticleContainer *container, size_t index);

    iterator(const &iterator_t other);

    ~iterator() = default;

    const reference operator*() const;

    reference operator*();

    const reference operator[](int offset) const;

    reference operator[](int offset);

    iterator_t &operator++();

    iterator_t &operator++(int);

    iterator_t &operator--();

    iterator_t &operator--(int);

    iterator_t &operator+=(int offset);

    iterator_t &operator-=(int offset);

    iterator_t &operator+(int offset) const;

    iterator_t &operator-(int offset) const;

    difference_type operator-(const iterator_t &other);

    bool operator==(const iterator_t &other) const;

    bool operator!=(const iterator_t &other) const;
    // we'll see if we need comparisons

   private:
    ParticleContainer *_container;
    size_t _index;
  };

 public:
  using iterator = ParticleContainer::iterator;
  using self_t = ParticleContainer;

  ParticleContainer();

  ParticleContainer(size_t size);

  ParticleContainer(const self_t &other);

  ParticleContainer(self_t &&other);

  ~ParticleContainer();

  self_t operator=(const self_t &rhs);

  self_t operator=(self_t &&rhs);

  bool operator==(self_t &rhs) const;

  bool operator!=(self_t &rhs) const;

  bool empty() const;

  bool size() const;

  iterator begin() const;

  // TODO add perfect forwarding
  iterator add(Particle p);

  iterator add(size_t count);

  iterator remove(iterator &it);

 private:
  std::vector<Particle> *_elements;
};

}  // namespace c3p

#endif  // C3P__PARTICLE_CONTAINER__INCLUDED
