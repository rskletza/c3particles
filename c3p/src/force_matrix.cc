#include <c3p/force_matrix.h>

#include<algorithm>

namespace c3p
{
    std::vector<Force> & ForceMatrix::operator[](size_t index)
    {
      return _matrix[index];
    }

    //iterates through the upper half of the matrix (split diagonally) and calculates the force p[x] has on p[y] and vice versa. For the second value, only an inversion is necessary, as the force is the inverted vector
    ForceMatrix & ForceMatrix::accumulate( std::function<Force(Particle&, Particle&, std::initializer_list<float>)> ff, std::initializer_list<float> params)
    {
      size_t x = 0, y = 0;
      //for is used here as it important to have access to the indices, which isn't possible any other way
      for(;x<_width;++x)
      {
        y = x;
        for(;y<_width;++y)
        {
          Force f = ff(_ps.particles()[x], _ps.particles()[y], params);
          std::cout << f << std::endl;
          _matrix[x][y] += f;
          _matrix[y][x] += f*(-1.0f);
        }
      }
    }

    ForceMatrix & ForceMatrix::reset()
    {
      //_matrix.insert(_matrix.begin(), _matrix.end(), std::vector<Force>(_width, glm::vec3(0,0,0)));
      std::fill(_matrix.begin(), _matrix.end(), std::vector<Force>(_width, glm::vec3(0,0,0))); 
          //TODO Force()
    }

std::ostream &operator<<(std::ostream &os, const ForceMatrix &fm)
{
  for (const std::vector<Force> & r : fm._matrix)
  {
    for (const Force & f : r)
    {
      std::cout << f << " ";
    }
    std::cout << "\n";
  }
  return os;
}
}
    

