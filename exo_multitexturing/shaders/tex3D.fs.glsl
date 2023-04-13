#version 410

uniform sampler2D uTex3D;


in vec3 vViewPosition;
in vec3 vViewNormal;
in vec2 vTexCoords;
in vec3 vPosPosition;

out vec4 fFragColor;



void main() {
    vec4 texColor = texture(uTex3D, vTexCoords);
    fFragColor = vec4(texColor.rgb,1.);       
}