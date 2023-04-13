#version 410

//variables uniformes
uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

//variables d'entrees
in vec3 vViewPosition;
in vec3 vViewNormal;
in vec2 vTexCoords;

//variable de sortie
out vec4 fFragColor;


void main() {
    vec2 fCorrectCoords = vec2(0.7+vTexCoords.x, 1-vTexCoords.y);
    fFragColor = texture(uEarthTexture,fCorrectCoords)+texture(uCloudTexture, fCorrectCoords);    
}