#version 300 es
precision highp float;

out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D texture1;

void main(){
     FragColor = texture(texture1,TexCoord);
//     FragColor = vec4(1.0,1.0,1.0,1.0);
}