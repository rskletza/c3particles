# c3particles: Modeling a Particle System in C++

c3particles is an implementation of a particle system model in C++ that separates the physical concepts and laws from the underlying graphics library. This enables a mathematical formulation of the forces influencing the particles.

## Summary

Particle systems are used in many different areas: most prominently in the entertainment industry in games and movies and for simulations and visualizations scientific research. No matter the area of application, the basic rules governing these systems are the same: the laws of physics. c3particles (cpp particles) implements a model of a particle system in C++ that separates the physical concepts and laws from the underlying graphics library. This enables a mathematical formulation of the forces influencing the particles. C++ is an optimal tool for this task, as it is a very mathematically expressive programming language that can be used to cleanly define formal concepts.

## System Diagram
![system diagram](https://github.com/rskletza/c3particles/blob/master/docs/images/complete-diagram.png)

## Requirements
C++14, OpenGL, GTK+3, CMake v3

## Installation
In the c3particles directory:
```BASH
mkdir build;
cd build;
cmake ..;
make;
```
## Execution
The execution file needs to be in the same parent directory as the shaders, so copy the shaders to the build directory:
In the build directory:
```BASH
cp -r c3p/src/shaders ./
```
In the build/c3p directory:
```BASH
./main
```

