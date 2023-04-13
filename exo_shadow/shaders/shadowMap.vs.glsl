#version 410

layout(location = 0) in vec3 aVertexPosition;

//a changer
uniform mat4 uMVPLight;

void main(){
    gl_Position = uMVPLight * vec4(aVertexPosition, 1.0);
}
