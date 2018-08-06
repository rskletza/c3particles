#version 330 core

layout(location=0) in vec3 vertexPosition_modelspace;
layout(location=1) in vec3 vertexColor;

uniform mat4 MVP;
uniform mat4 oldMVP;

smooth out vec4 vPosition;
smooth out vec4 vPrevPosition;

out vec3 fragmentColor;

void main()
{
  vPosition = MVP * gl_Position;
  vPrevPosition = oldMVP * gl_Position;
    gl_Position = MVP * vec4(vertexPosition_modelspace,1);
    fragmentColor = vertexColor;
}
