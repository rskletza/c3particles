project (c3p)

find_package(OpenGL REQUIRED)

#use pkgconfig to detect gtk headers/library files
find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK REQUIRED gtk+-3.0)

#include thread libraries
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

include_directories(
  ${GTK_INCLUDE_DIRS}
  include/
)

link_directories(
  ${GTK_LIBRARY_DIRS}
  )

set(ALL_LIBS
    ${OPENGL_LIBRARY}
    glfw
    GLEW
)

add_definitions(
    -DTW_STATIC
    -DTW_NO_LIB_PRAGMA
    -DTW_NO_DIRECT3D
    -DGLEW_STATIC
    -D_CRT_SECURE_NO_WARNINGS
    ${GTK_CFLAGS_OTHER}
)

add_executable(
    main
    src/main.cc
    src/particle_system.cc
    src/particle_renderer.cc
    src/particle_functions.cc
    src/newtonian_objects.cc
    #    src/control_window.c
    #    src/particle_container.cc
    src/common/shader.cc
)

target_link_libraries(main
  ${GTK_LIBRARIES}
  Threads::Threads
	${ALL_LIBS}
)

SOURCE_GROUP(common REGULAR_EXPRESSION "src/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION "src/shaders/.*$" )

