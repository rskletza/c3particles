uniform sampler2D texture;
varying vec2 texCoord;

void main(){
	float blurSize = 0.01;
    //gl_FragColor = gl_Color;
    gl_FragColor = texture2D(texture, texCoord);
    //gl_FragColor += texture2D(texture, texCoord);
    
   gl_FragColor += texture2D(texture, vec2(texCoord.x - 4.0*blurSize, texCoord.y)) * 0.05;
   gl_FragColor += texture2D(texture, vec2(texCoord.x - 3.0*blurSize, texCoord.y)) * 0.09;
   gl_FragColor += texture2D(texture, vec2(texCoord.x - 2.0*blurSize, texCoord.y)) * 0.12;
   gl_FragColor += texture2D(texture, vec2(texCoord.x - blurSize, texCoord.y)) * 0.15;
   gl_FragColor += texture2D(texture, vec2(texCoord.x, texCoord.y)) * 0.16;
   gl_FragColor += texture2D(texture, vec2(texCoord.x + blurSize, texCoord.y)) * 0.15;
   gl_FragColor += texture2D(texture, vec2(texCoord.x + 2.0*blurSize, texCoord.y)) * 0.12;
   gl_FragColor += texture2D(texture, vec2(texCoord.x + 3.0*blurSize, texCoord.y)) * 0.09;
   gl_FragColor += texture2D(texture, vec2(texCoord.x + 4.0*blurSize, texCoord.y)) * 0.05;
}
