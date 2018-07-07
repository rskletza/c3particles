//template expressions
//constexpr
//
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <experimental/random>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
#include <common/shader.hpp>
//#include <common/controls.hpp>
#include <src/Particle.cpp>

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);

	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glPointSize(15.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

    GLuint shaders = LoadShaders( "../shaders/simple_vert_shader.glsl", "../shaders/simple_frag_shader.glsl" );
    
	// Get a handles for shaders
	GLuint MatrixID = glGetUniformLocation(shaders, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
	
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(0,0,-9), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
  glm::mat4 mvp        = Projection * View * Model; // Remember, matrix multiplication is the other way around

    std::srand(std::time(nullptr));

    int width, height;
    double xpos, ypos;
    Particle p;
    std::vector<Particle> particles;
    particles.push_back(p);
    for (int i = 0; i<5; ++i)
    {
        particles.push_back(std::move(Particle()));
    }

	do{
        //clear the screen and clear the depth
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
        
//        glClearColor(0.0f, 0.0f, rand()/(float)RAND_MAX, 0.0f);
//
        //use own shader
        glUseProgram(shaders);
        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        //translate xpos and ypos to modelspace
        glfwGetFramebufferSize(window, &width, &height);
        glfwGetCursorPos(window, &xpos, &ypos);
        xpos -= width/2;
        ypos -= height/2;

        //dirty hack to scale movement. TODO invert projection matrix or something
        xpos /= 100;
        ypos /= 100;
        
        
        for (int i = 0; i<5; ++i)
        {
//            particles[i].location(-xpos+i, -ypos+i);
            particles[i].followMouse(-xpos, -ypos);
            particles[i].draw(mvp, MatrixID);
        }


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

