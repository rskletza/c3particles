project (c3p)

find_package(OpenGL REQUIRED)

include_directories(
# external/glfw-3.1.2/include/
# external/glm-0.9.7.1/
# external/glew-1.13.0/include/
# .
  include/
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
  GLEW
#	GLEW_1130
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# User playground
add_executable(
  c3p.bin
	src/main.cpp
  src/particle_system.cpp
  src/particle_renderer.cpp
  src/common/shader.cpp
)
target_link_libraries(main
	${ALL_LIBS}
)

SOURCE_GROUP(common REGULAR_EXPRESSION "src/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION "src/shaders/.*$" )

