#version 410

layout(location = 0) in vec3 aVertexPosition;

//a changer
uniform mat4 uMVPMatrix;

void{
    gl_Position = MVP * vec4(Position, 1.0);
}
