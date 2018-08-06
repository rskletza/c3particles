#version 150
//#version 330 core

in vec3 fragmentColor;

smooth in vec4 vPosition;
smooth in vec4 vPrevPosition;

//output data
out vec3 color;
out vec2 oVelocity;

void main()
{
    vec2 a = (vPosition.xy / vPosition.w) * 0.5 + 0.5;
    vec2 b = (vPrevPosition.xy / vPrevPosition.w) * 0.5 + 0.5;
    oVelocity = a - b;
    color = fragmentColor; //vec3(1,1,1);
}
