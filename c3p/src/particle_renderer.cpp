
#include <c3p/particle_renderer.h>

#include <algorithm>

namespace c3p {

class ParticleSystem;

using ParticleContainer = std::vector<Particle>;

//    ParticleRenderer::ParticleRenderer() = delete;

ParticleRenderer::ParticleRenderer(const ParticleSystem &ps)
    : _particlecontainer(ps._particles) {}

ParticleRenderer::~ParticleRenderer() = default;

// assignment and comparison should not be needed since there should always only
// be one renderer per particle system  should the renderer then be a member of
// the particle system?

// calculate color and vertex buffers for each particle and pass them to OpenGL
void ParticleRenderer::render(glm::mat4 &mvp, GLuint MatrixID) {
  // for each particle. for now will use
  for (Particle &p : *_particlecontainer) {
    GLuint colorbuffer;
    GLuint vertexbuffer;

    // void fillVertexBuffer()
    GLfloat vertex_buffer_data[3] = {p.location[0], p.location[1],
                                     p.location[2]};
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
                 vertex_buffer_data, GL_STATIC_DRAW);

    // void fillColorBuffer()
    GLfloat color_buffer_data[3] = {p.color[0], p.color[1], p.color[2]};
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,        // attribute 0. must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(1, // attribute. must match the layout in the shader.
                          3, // size
                          GL_FLOAT, // type
                          GL_FALSE, // normalized?
                          0,        // stride
                          (void *)0 // array buffer offset
    );
    glDrawArrays(GL_POINTS, 0, 1); // 1 vertex
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
  }
}

} // namespace c3p
