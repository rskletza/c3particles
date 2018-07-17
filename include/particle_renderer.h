#ifndef C3P__PARTICLE_RENDERER__INCLUDED
#define C3P__PARTICLE_RENDERER__INCLUDED

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "include/particle_system.h"

//first, bind renderer to system
//auto pr = ParticleRenderer(particlesystem);
//
//then render particles (fill color and vertex buffer for each particle
//pass the model-view-projection matrix and handle for shaders
//pr.render(mvp, MatrixID)

namespace c3p {

struct Particle; //TODO why isn't this clear from the particle_system header?
class ParticleSystem;

using ParticleContainer = std::vector<Particle>;

class ParticleRenderer
{
  public:
    ParticleRenderer() = delete;
    ParticleRenderer(const ParticleSystem &);
    ~ParticleRenderer();

    //assignment and comparison should not be needed since there should always only be one renderer per particle system
    //should the renderer then be a member of the particle system?
    
    //calculate color and vertex buffers for each particle and pass them to OpenGL
    void render(glm::mat4 & mvp, GLuint MatrixID);

  private:
    ParticleContainer * _particlecontainer;
};

} //namespace c3p

#endif //C3P__PARTICLE_RENDERER__INCLUDED
