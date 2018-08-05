#ifndef C3P__FORCE_MATRIX__INCLUDED
#define C3P__FORCE_MATRIX__INCLUDED

#include <vector>
#include <functional>

#include <c3p/particle_system.h>
#include <c3p/newtonian_objects.h>

namespace c3p
{
class ForceMatrix
{
  friend std::ostream &operator<<(std::ostream &os, const ForceMatrix &fm);

  public:
    ForceMatrix() = delete;

    ForceMatrix(ParticleSystem & ps)
      :_ps(ps)
      ,_width(ps.size())
      ,_matrix(ps.size(), std::vector<Force>(ps.size()))
      { }

    ~ForceMatrix() = default;

    ForceMatrix(const ForceMatrix & other)
      :_ps(other._ps)
      ,_width(other._width)
      ,_matrix(other._width, std::vector<Force>(other._width))
    { }

    ForceMatrix & operator=(const ForceMatrix & rhs) = default;
//    {
//      _matrix = rhs._matrix;
//      _width = rhs._width;
//      _ps = rhs._ps;
//      _
//    }

    std::vector<Force> & operator[](size_t index);

    //iterates through the upper half of the matrix (split diagonally) and calculates the force p[x] has on p[y] and vice versa. For the second value, only an inversion is necessary, as the force is the inverted vector
    ForceMatrix & accumulate(std::function<Force(Particle&, Particle&, std::initializer_list<float> )> ff, std::initializer_list<float> params);

    ForceMatrix & reset();

  private:
    std::vector<std::vector<Force> > _matrix;
    size_t _width;
    c3p::ParticleSystem & _ps;
};

std::ostream &operator<<(std::ostream &os, const ForceMatrix &fm);
}


#endif //C3P__FORCE_MATRIX__INCLUDED
