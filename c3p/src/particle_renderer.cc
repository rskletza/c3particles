
#include <c3p/particle_renderer.h>

#include <algorithm>

namespace c3p
{
class ParticleSystem;

using ParticleContainerFk = std::vector<Particle>;

//    ParticleRenderer::ParticleRenderer() = delete;

ParticleRenderer::ParticleRenderer(const ParticleSystem &ps)
    : _particlecontainer(ps._particles)
{
}

ParticleRenderer::~ParticleRenderer() = default;

// assignment and comparison should not be needed since there should always only
// be one renderer per particle system
void ParticleRenderer::renderCubes()
{
  for (const Particle &p : _particlecontainer)
    {
      GLuint colorbuffer;
      GLuint vertexbuffer;
      GLuint elementbuffer;

      float w = p.size / 2.0f;
      glm::vec3 pos = p.location;
      GLfloat vertex_buffer_data[] = {
          // front
          (pos.x - w), (pos.y - w), (pos.z + w),  // bottom left 0
          (pos.x + w), (pos.y - w), (pos.z + w),  // bottom right 1
          (pos.x + w), (pos.y + w), (pos.z + w),  // top left 2
          (pos.x - w), (pos.y + w), (pos.z + w),  // top right 3

          // back
          (pos.x - w), (pos.y - w), (pos.z - w),  // bottom left 4
          (pos.x + w), (pos.y - w), (pos.z - w),  // bottom right 5
          (pos.x + w), (pos.y + w), (pos.z - w),  // top left 6
          (pos.x - w), (pos.y + w), (pos.z - w),  // top right 7
      };
      glGenBuffers(1, &vertexbuffer);
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
                   vertex_buffer_data, GL_STATIC_DRAW);

      glm::vec3 color = p.color;
      GLfloat color_buffer_data[24];
      for (int i = 0; i < 24; i += 3)
        {
          color_buffer_data[i] = color[0];
          color_buffer_data[i + 1] = color[1];
          color_buffer_data[i + 2] = color[2];
        }
      glGenBuffers(1, &colorbuffer);
      glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data),
                   color_buffer_data, GL_STATIC_DRAW);

      GLuint cube_elements[] = {
          // front
          0, 1, 2, 2, 3, 0,
          // right
          1, 5, 6, 6, 2, 1,
          // back
          7, 6, 5, 5, 4, 7,
          // left
          4, 0, 3, 3, 7, 4,
          // bottom
          4, 5, 1, 1, 0, 4,
          // top
          3, 2, 6, 6, 7, 3,
      };
      glGenBuffers(1, &elementbuffer);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements),
                   cube_elements, GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      glVertexAttribPointer(
          0,         // attribute 0. must match the layout in the shader.
          3,         // size
          GL_FLOAT,  // type
          GL_FALSE,  // normalized?
          0,         // stride
          (void *)0  // array buffer offset
          );

      // 2nd attribute buffer : colors
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
      glVertexAttribPointer(
          1,         // attribute. must match the layout in the shader.
          3,         // size
          GL_FLOAT,  // type
          GL_FALSE,  // normalized?
          0,         // stride
          (void *)0  // array buffer offset
          );

      // Push each element in buffer_vertices to the vertex shader
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
      int size;
      glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
      glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);

      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);

      glDeleteBuffers(1, &elementbuffer);
      glDeleteBuffers(1, &vertexbuffer);
      glDeleteBuffers(1, &colorbuffer);
    }
}

// calculate color and vertex buffers for each particle and pass them to OpenGL
void ParticleRenderer::renderPoints(float size)
{
  glPointSize(size);
  // for each particle. for now will use
  for (const Particle &p : _particlecontainer)
    {
      GLuint colorbuffer;
      GLuint vertexbuffer;

      GLfloat vertex_buffer_data[3] = {p.location[0], p.location[1],
                                       p.location[2]};
      glGenBuffers(1, &vertexbuffer);
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
                   vertex_buffer_data, GL_STATIC_DRAW);

      GLfloat color_buffer_data[3] = {p.color[0], p.color[1], p.color[2]};
      // GLfloat color_buffer_data[3] = {(p.location[0]+30)/60,
      // (p.location[1]+30)/60,
      //  (p.location[2]+30)/60};
      glGenBuffers(1, &colorbuffer);
      glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data),
                   color_buffer_data, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      glVertexAttribPointer(
          0,         // attribute 0. must match the layout in the shader.
          3,         // size
          GL_FLOAT,  // type
          GL_FALSE,  // normalized?
          0,         // stride
          (void *)0  // array buffer offset
          );

      // 2nd attribute buffer : colors
      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
      glVertexAttribPointer(
          1,         // attribute. must match the layout in the shader.
          3,         // size
          GL_FLOAT,  // type
          GL_FALSE,  // normalized?
          0,         // stride
          (void *)0  // array buffer offset
          );
      glDrawArrays(GL_POINTS, 0, 1);  // 1 vertex
      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);

      glDeleteBuffers(1, &vertexbuffer);
      glDeleteBuffers(1, &colorbuffer);
    }
}

}  // namespace c3p
