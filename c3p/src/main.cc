// expression templates
// constexpr
//
#include <algorithm>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
//#include <execution>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtk/gtk.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <c3p/common/camera.h>
#include <c3p/common/shader.h>

#include <c3p/control_window.h>
#include <c3p/newtonian_objects.h>
#include <c3p/particle_functions.h>
#include <c3p/particle_renderer.h>
#include <c3p/particle_system.h>

using namespace c3p;

GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action,
                           int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

int setupGLFW();

// settings
// const unsigned int SCR_WIDTH = 1280;
// const unsigned int SCR_HEIGHT = 1400;
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 1400;

// variables for camera control
Camera camera(glm::vec3(0.0f, 0.0f, 100.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool lmousedown = false;
glm::mat4 Model = glm::mat4(1.0f);
float deltaTime = 0.0f;  // time between current frame and last frame
float lastFrame = 0.0f;

int main(void)
{
  ////////////////// initialize struct to hold particle system control data
  //////////

  ParticleControlData*
      ctl_p;  // struct to hold the data from the control window
  struct ParticleControlData ctl;
  ctl_p = &ctl;
  initParticleControls(ctl_p);

  ///////////////// start a thread that runs the gui ///////////////////////
  std::thread ctl_window([ctl_p] {

    GtkApplication* app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), (gpointer)ctl_p);
    status = g_application_run(G_APPLICATION(app), 0, nullptr);
    g_object_unref(app);
  });

  //////////////// start a thread that runs the viewbox ////////////////////

  std::thread view([ctl_p] {

    /////////////// set up GLFW and OpenGL /////////////////////
    setupGLFW();

    // black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint shaders = LoadShaders("../shaders/blur_vert_shader.glsl",
                                 "../shaders/blur_frag_shader.glsl");

    // Get a handle for shaders
    GLuint MatrixID = glGetUniformLocation(shaders, "MVP");
    GLuint oldMatrixID = glGetUniformLocation(shaders, "oldMVP");

    glm::mat4 mvp;  // model view projection matrix

    //////////////// initialize particle system /////////////////////////

    c3p::ParticleSystem ps(100);
    ps.setStartConfiguration();
    c3p::ParticleRenderer p_renderer(ps);
//    c3p::ForceMatrix fm(ps); //force matrix not active
    float cage_width_half =
        30;  // contain the particles in a cage with side length 200

    /////////////// execution loop ////////////////
    do
      {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //check for camera movement input
        processInput(window);

        //check whether particle trails are turned on and clear buffer if not
        if (ctl_p->trail_checkbtn)
          {
            glClear(GL_DEPTH_BUFFER_BIT);
          }
        else
          {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          }

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        // Accept fragment if it is closer to the camera than the former one
        glDepthFunc(GL_LESS);

        // use own shader
        glUseProgram(shaders);

        glm::mat4 Projection = glm::perspective(
            glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f, 1000.0f);
        glm::mat4 View = camera.GetViewMatrix();

        // pass old mvp to shaders
        glUniformMatrix4fv(oldMatrixID, 1, GL_FALSE, &mvp[0][0]);

        mvp = Projection * View * Model;

        // pass mvp to shaders
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        // TODO measure time since last swap buffers (std::chrono) to decouple time from framerate

        if (ctl_p->restart_btn)
          {
            ps.reset();
            ctl_p->restart_btn = 0;
          }

        if (ctl_p->reverse_btn)
          {
            ps.reverse();
            ctl_p->reverse_btn = 0;
          }

        // update settings from gui
        ps.setGexponent(ctl_p->g_scale);
        ps.requestNewSize((size_t)ctl_p->num_particles);

        if (!ctl_p->pause_btn)
          {
            //          std::for_each(std::execution::parallel_unsequenced_policy,
            //          ps.begin(), ps.end(), [&ps, ctl_p,
            //          cage_width_half](c3p::Particle& p) {
            std::for_each(ps.begin(), ps.end(), [&ps, ctl_p, cage_width_half](
                                                    c3p::Particle& p) {
              if (ctl_p->g_checkbtn)
                {
                  // apply accumulated gravitational force between all
                  // particles and this particle
                  p << c3p::accumulate(
                      p, ps.particles(), {ps.g_constant()},
                      c3p::gravity);  // gravitational forces between particles
                }

              if (ctl_p->s_checkbtn)
                {
                  // apply accumulated spring force between all
                  // particles and this particle
                  p << c3p::accumulate(
                      p, ps.particles(),
                      {(float)ctl_p->sl_scale, (float)ctl_p->s_scale},
                      c3p::spring);
                }

              if (ctl_p->c_spring_checkbtn)
                {
                  // apply spring force between center and this particle
                  p << spring(p, Particle(),
                              {(float)ctl_p->sl_scale, (float)ctl_p->s_scale});
                }
              if (ctl_p->c_gravity_checkbtn)
                {
                  // apply gravitational force between center and this particle
                  p << gravity(p, Particle(100.0f), {ps.g_constant()});
                }
              if (ctl_p->c_attract_checkbtn)
                {
                  // apply attraction force between center and this particle
                  p << simple_attract(p, Particle(10.0), {0.001});
                }

              // check for collision with cube and calculate reflective force
              // TODO extract into function or add to ParticleSystem class as _boundary
              if (ctl_p->cage_checkbtn)
                {
                  if (p.location.x >= cage_width_half)
                    {
                      p.velocity =
                          glm::reflect(p.velocity, glm::vec3(-1, 0, 0));
                      p.acceleration =
                          glm::reflect(p.acceleration, glm::vec3(-1, 0, 0));
                    }

                  else if (p.location.x <= -cage_width_half)
                    {
                      p.velocity = glm::reflect(p.velocity, glm::vec3(1, 0, 0));
                      p.acceleration =
                          glm::reflect(p.acceleration, glm::vec3(1, 0, 0));
                    }

                  else if (p.location.y >= cage_width_half)
                    {
                      p.velocity =
                          glm::reflect(p.velocity, glm::vec3(0, -1, 0));
                      p.acceleration =
                          glm::reflect(p.acceleration, glm::vec3(0, -1, 0));
                    }

                  else if (p.location.y <= -cage_width_half)
                    {
                      p.velocity = glm::reflect(p.velocity, glm::vec3(0, 1, 0));
                      p.acceleration =
                          glm::reflect(p.acceleration, glm::vec3(0, 1, 0));
                    }

                  else if (p.location.z >= cage_width_half)
                    {
                      p.velocity = glm::reflect(p.velocity, glm::vec3(0, 0, 1));
                      p.acceleration =
                          glm::reflect(p.acceleration, glm::vec3(0, 0, 1));
                    }

                  else if (p.location.z <= -cage_width_half)
                    {
                      p.velocity =
                          glm::reflect(p.velocity, glm::vec3(0, 0, -1));
                      p.acceleration =
                          glm::reflect(p.acceleration, glm::vec3(0, 0, -1));
                    }
                }

            });

            // update all the particles
            ps.update();
//            std::for_each(ps.begin(), ps.end(),
//                          [](c3p::Particle& p) { update(p); });
          }

        p_renderer.renderCubes();

        glfwSwapBuffers(window);
        glfwPollEvents();

      }  // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
  });

  ctl_window.join();
  view.join();

  return 0;
}

int setupGLFW()
{
  // Initialise GLFW
  if (!glfwInit())
    {
      fprintf(stderr, "Failed to initialize GLFW\n");
      getchar();
      return -1;
    }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context

  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "c3particles", NULL, NULL);

  if (window == NULL)
    {
      fprintf(stderr,
              "Failed to open GLFW window. If you have an Intel GPU, they are "
              "not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
      getchar();
      glfwTerminate();
      return -1;
    }

  glfwMakeContextCurrent(window);

  glewExperimental = true;  // Needed for core profile Initialize GLEW
  if (glewInit() != GLEW_OK)
    {
      fprintf(stderr, "Failed to initialize GLEW\n");
      getchar();
      glfwTerminate();
      return -1;
    }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    lmousedown = true;
  else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    lmousedown = false;
}

void processInput(GLFWwindow* window)
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
  // make sure the viewport matches the new window dimensions; note that width
  // and
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

  float xoffset = (xpos - lastX) * 0.1;
  float yoffset = (lastY - ypos) *
                  (-0.1);  // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  if (lmousedown)
    {
      glm::mat4 trans = glm::mat4(1.0f);
      trans =
          glm::rotate(trans, glm::radians(yoffset),
                      glm::cross(camera.Position, glm::vec3(0.0f, 1.0f, 0.0f)));
      trans = glm::rotate(trans, glm::radians(xoffset),
                          glm::vec3(0.0f, 1.0f, 0.0f));
      Model = Model * trans;
      //    camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(yoffset);
}
