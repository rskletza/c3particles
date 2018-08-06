varying vec2 texCoord;
void main(){
    gl_Position = gl_ModelViewProjectionMatrix * gl_Position; //The order in which you times matrices and vertices is IMPORTANT.

    texCoord = gl_MultiTexCoord0.xy;

    //gl_FrontColor = gl_Color;   //These lines just pass on the color value to the fragment shader.
    //gl_BackColor = gl_Color;
}
