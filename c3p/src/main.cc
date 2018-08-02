// expression templates
// constexpr
//
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <experimental/random>
#include <iostream>
#include <thread>
#include <vector>

#include <gtk/gtk.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <c3p/common/shader.h>
#include <c3p/common/camera.h>

#include <c3p/control_window.h>
#include <c3p/newtonian_objects.h>
#include <c3p/particle_functions.h>
#include <c3p/particle_renderer.h>
#include <c3p/particle_system.h>
//#include <c3p/particle_container.h>

using namespace c3p;

GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
//{
//  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//    mousedown = true;
//  else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
//    mousedown = false;
//}

int main(void)
{
  ControlData* ctl_p;  // struct to hold the data from the control window
  struct ControlData ctl;
  ctl_p = &ctl;
  initControls(ctl_p);

  std::thread ctl_window([ctl_p] {
    GtkApplication* app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), (gpointer)ctl_p);
    status = g_application_run(G_APPLICATION(app), 0, nullptr);
    g_object_unref(app);
  });

  std::thread view([ctl_p] {

    // Initialise GLFW
    if (!glfwInit())
      {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
      }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "c3particles", NULL, NULL);
    //window = glfwCreateWindow(1280, 1400, "c3particles", NULL, NULL);

    if (window == NULL)
      {
        fprintf(
            stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are "
            "not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
      }

    glewExperimental = true;  // Needed for core profile Initialize GLEW
    if (glewInit() != GLEW_OK)
      {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
      }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

//    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearAccum(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint shaders = LoadShaders("../shaders/simple_vert_shader.glsl",
                                 "../shaders/simple_frag_shader.glsl");

    // Get a handle for shaders
    GLuint MatrixID = glGetUniformLocation(shaders, "MVP");

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1
    // unit
    // <-> 100 units
    glm::mat4 Projection =
        glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
    // Or, for an ortho camera :
    // glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
    // // In world coordinates

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        //glm::vec3(0, 0, -200),  // Camera is at (x,y,z), in World Space
        glm::vec3(0, 0, -50),  // Camera is at (x,y,z), in World Space
        glm::vec3(0, 0, 0),     // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; 

    glPointSize(5.0f);
    c3p::ParticleSystem ps(50);
    ps.setRandom();
    c3p::ParticleRenderer p_renderer(ps);

    do
      {
        //TODO copy control struct
        //synchronize
        // clear the screen and clear the depth

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        if (ctl_p->trail_checkbtn)
          {
            glClear(GL_DEPTH_BUFFER_BIT);
          }
        else
          {
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          }
        //copy current accumulation buffer to color buffer multiplied by factor
        glClear(GL_ACCUM_BUFFER_BIT);
        glAccum(GL_RETURN, 0.95f);
        glAccum(GL_MULT, 0.5f);


//        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        // use own shader
        glUseProgram(shaders);


        // TODO physics engine in own thread --> sleep
        // TODO measure time since last swap buffers (std::chrono)

        if (ctl_p->restart_btn)
          {
            ps.reset();
            ctl_p->restart_btn = 0;  // critical! TODO
          }

        if (ctl_p->reverse_btn)
        {
          ps.reverse();
          ctl_p->reverse_btn = 0;  // critical! TODO
        }

        // update gravitational constant
        ps.setGexponent(ctl_p->g_scale);

        //      std::transform(ps.begin(), ps.end(), ps.begin(),
        std::for_each(ps.begin(), ps.end(), [&ps, ctl_p](c3p::Particle& p) {
          if (ctl_p->g_checkbtn)
            {
              p << c3p::accumulate(
                  p, ps.particles(), {ps.g_constant()},
                  c3p::gravity);  // gravitational forces between particles
            }

          if (ctl_p->g_center_checkbtn)
            {
              // attract to center
              p << gravity(p, Particle(100.0f), {ps.g_constant()});
            }

          if (ctl_p->s_checkbtn)
            {
              p << c3p::accumulate(p, ps.particles(), {10, 1}, c3p::spring);
              Force s = spring(p, Particle(), {5, 0.001});
              std::cout << s << std::endl;
              p << std::move(s);
            }
        });

        ps.update();
        //have for out here
        //p_renderer.renderPoints(mvp, MatrixID);
        p_renderer.renderCubes(mvp, MatrixID);
        
        // update camera from controls
//        View = glm::lookAt(
//            glm::vec3(0, 0, ctl_p->dolly_scale),
//            glm::vec3(ctl_p->pan_scale, ctl_p->tilt_scale, 0),
//            glm::vec3(0, 1,
//                      0)  // Head is up (set to 0,-1,0 to look upside-down)
//            );
        Projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        View = camera.GetViewMatrix();
        glm::mat4 mvp = Projection * View * Model;

        // Swap buffers

        //glReadBuffer(GL_FRONT);
        //glAccum(GL_ACCUM, 0.9f);
        glfwSwapBuffers(window);
        glAccum(GL_LOAD, 0.9f);
        glfwPollEvents();

      }  // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
  });

  ctl_window.join();
  view.join();
  //  gtk_window_close(window);

  return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  std::cout << "scrolling" << std::endl;
    camera.ProcessMouseScroll(yoffset);
}

