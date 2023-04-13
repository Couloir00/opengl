#version 410

uniform sampler2D uMoonTexture;

in vec3 vViewPosition;
in vec3 vViewNormal;
in vec2 vTexCoords;

out vec4 fFragColor;



void main() {
    vec4 texColor = texture(uMoonTexture, vTexCoords);
    fFragColor = vec4(texColor.rgb,1.);       
}