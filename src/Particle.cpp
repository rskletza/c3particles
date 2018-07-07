//#ifndef CPPPC_PROJECT__PARTICLE_H_INCLUDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

using vec = std::array<float, 3>;

vec add(vec a, vec b)
{
    vec c;
    for (int i = 0; i<3; ++i)
    {
        c[i] = a[i] + b[i];
    }
    return c;
}

glm::vec3 vectovec3(vec & vector, glm::vec3 glmvec)
{
    glmvec.x = vector[0];
    glmvec.y = vector[1];
    glmvec.z = vector[2];
    return glmvec;
}

vec vec3tovec(glm::vec3 glmvec, vec & vector)
{
    vector[0] = glmvec.x;
    vector[1] = glmvec.y;
    vector[2] = glmvec.z;
    return vector;
}

class Particle 
{
  public:
    using self_t = Particle;

    Particle()
    {   
        static GLfloat g_color_buffer_data[3];
        static GLfloat g_vertex_buffer_data[3];
        for (int i = 0; i < 3; ++i)
        {
            float r = rand();
            r = r/ (float) RAND_MAX;
            _color[i] = r;
            g_color_buffer_data[i] = r;
            _location[i] = r*12-6;
        }
        glGenBuffers(1, &_colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

        fillVertexBuffer();
    }

    Particle(vec loc)
        :_location(loc) 
    {   
        static GLfloat g_color_buffer_data[3];
        for (int i = 0; i < 3; ++i)
        {
            float r = rand();
            r = r/ (float) RAND_MAX;
            _color[i] = r;
            g_color_buffer_data[i] = r;
        }
        glGenBuffers(1, &_colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

        fillVertexBuffer();
    }

    ~Particle() = default;

    Particle(const self_t & other) = default;
    self_t & operator=(const self_t & rhs) = default;

    //move constructor and assignment not necessary, because so small
    //yeah but if I make a ton of them?

//    vec color() 
//    {
//        return _color;
//    }
//
//    decltype(auto) color()
//    {
//        return _colorbuffer;
//    }
//    
//
    vec location() const
    {
        return _location;
    }

    vec location(float x, float y)
    {
        _location = {x,y,0};
    }
//
//    vec acceleration()
//    {
//        return _acceleration;
//    }
//
//    vec velocity()
//    {
//        return _velocity;
//    }
//
    void update()
    {
        _velocity = add(_velocity, _acceleration);
        _location = add(_location, _velocity);
    }
    void followMouse(double xmouse, double ymouse)
    {
        vec3 mouse = {xmouse, ymouse, 0.0};
        //expression template?
        vec3 loc = vectovec3(_location, loc);
        vec3 dir = mouse - loc;
//        normalize(dir);
        dir *= 0.001;
        vec3tovec(dir, _acceleration);
        update();
    }

    //write location to vertex buffer (only works for single vertex particles)
    void fillVertexBuffer()
    { 
        GLfloat buffer_data[3] = {_location[0], _location[1], _location[2]};
        glGenBuffers(1, &_vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
    }
    
    void draw(glm::mat4 & mvp, GLuint MatrixID)
    {
        fillVertexBuffer();

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
        glVertexAttribPointer(
           0,                  // attribute 0. must match the layout in the shader.
           3,                  // size
           GL_FLOAT,           // type
           GL_FALSE,           // normalized?
           0,                  // stride
           (void*)0            // array buffer offset 
                );
        
        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, _colorbuffer);
        glVertexAttribPointer(
            1,                  // attribute. must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        glDrawArrays(GL_POINTS, 0, 1); //1 vertex
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    }


  private:
    vec _location;
    vec _velocity;
    vec _acceleration;
    vec _color;
    
    GLuint _colorbuffer;
    GLuint _vertexbuffer; 


};
