#version 330 core

in vec3 fragmentColor;
//output data
out vec3 color;

void main()
{
    color = fragmentColor; //vec3(1,1,1);
}
