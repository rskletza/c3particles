#ifndef C3P__PARTICLE_RENDERER__INCLUDED
#define C3P__PARTICLE_RENDERER__INCLUDED

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <c3p/newtonian_objects.h>
#include <c3p/particle_system.h>

// first, bind renderer to system
// auto pr = ParticleRenderer(particlesystem);
//
// then render particles (fill color and vertex buffer for each particle
// pass the model-view-projection matrix and handle for shaders
// pr.render(mvp, MatrixID)

namespace c3p
{
class ParticleSystem;

using ParticleContainerFk = std::vector<Particle>;

class ParticleRenderer
{
 public:
  ParticleRenderer() = delete;
  ParticleRenderer(const ParticleSystem &);
  ~ParticleRenderer();

  // Assignment and comparison should not be needed since there should always
  // only be one renderer per particle system  should the renderer then be a
  // member of the particle system? --> no, separation of concerns

  /// Calculate color and vertex buffers for each particle and pass them to
  /// OpenGL
  void renderPoints(float);
  void renderCubes();

 private:
  const ParticleContainerFk &_particlecontainer;
};

}  // namespace c3p

#endif  // C3P__PARTICLE_RENDERER__INCLUDED
