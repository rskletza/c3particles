//#ifndef CPPPC_PROJECT__PARTICLE_H_INCLUDED
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace c3p {


//using vec = std::array<float, 3>;
using vec = glm::vec3;

class Particle 
{
  public:
    using self_t = Particle;

    Particle()
    {   
        static GLfloat g_color_buffer_data[3];
        for (int i = 0; i < 3; ++i)
        {
            float r = rand();
            r = r/ (float) RAND_MAX;
            _color[i] = r;
            _location[i] = r*10-5;
            _origin = _location;
//            _mass = r*100;
            _mass = r*10 + 50;
        }
    }

    ~Particle() = default;

    Particle(const self_t & other) = default;
    self_t & operator=(const self_t & rhs) = default;

    //move constructor and assignment not necessary, because so small
    //yeah but if I make a ton of them?

    vec origin() const
    {
        return _origin;
    }

    
    void clear() //clear acceleration before next frame (accumulative)
    {
        _acceleration = glm::vec3(0.0, 0.0, 0.0);
    }

    void update(float deltaT) //float because glm::vec3 can't deal with double
    {
        //v(t)= Int(acc) = acc*t + C
        //C is the integration constant, which is the velocity at the previous point in time
        //--> v(t) = a*t + v(t-1)
        //t in this case is equal to 1, as I am using frames to measure time (TODO frames are not always constant, use actual time eg seconds with delta glfwGetTime
        //as a result, the velocity v(t) = acc*1 + v(t-1)
        _velocity += _acceleration*deltaT; 
        //s(t) = Int(velocity) = Int(acc*t + C) = (a*t^2)/2 + C*t + C1
        //C1 is the location at the previous point in time
        //--> s(t) = (a*t^2)/2 + v(t) + s(t-1)
        //again, t is equal to 
        _location += (_acceleration*deltaT)/2.0f + _velocity;
    }

    void gravitate(vec point, double strength)
    {
        vec dir = point - _location;
        glm::normalize(dir);
        dir *= strength;
        applyForce(dir);
    }

    //apply a force (jerk)
    void applyForce(glm::vec3 force)
    {
        force /= _mass; //f = m*a
        _acceleration += force;
    }

    //write location to vertex buffer (only works for single vertex particles)
    void fillVertexBuffer()
    { 
        GLfloat buffer_data[3] = {_location[0], _location[1], _location[2]};
        glGenBuffers(1, &_vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
    }

    void fillColorBuffer()
    {
        GLfloat buffer_data[3] = {_color[0], _color[1], _color[2]};
        glGenBuffers(1, &_colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
    }
    
    //blend mode --> spuren
    //fft to plug in audio
    void draw(glm::mat4 & mvp, GLuint MatrixID)
    {
        update(glfwGetTime());
        fillVertexBuffer();
        fillColorBuffer();

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

        (*this).clear();
    }


  private:
    vec _origin;
    vec _location;
    vec _velocity;
    vec _acceleration;// = glm::vec3(0,0,0);
    vec _color;
    float _mass = 1.0;
    float _ttl;
    
    //part of the Projection System
    GLuint _colorbuffer;
    GLuint _vertexbuffer; 


};

} //namespace c3p
