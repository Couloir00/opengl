#version 410

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexNormal;
layout(location = 2) in vec2 aVertexTexCoords;

//variables uniformes
uniform mat4 uMVPMatrix;
uniform mat4 uMVMatrix;
uniform mat4 uNormalMatrix;

//variables de sorties
out vec3 vViewPosition;
out vec3 vViewNormal;
out vec2 vTexCoords;


void main() {
    // Passage en coordonnées homogènes
    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 1);

    // Calcul des valeurs de sortie
    vViewPosition = vec3(uMVMatrix * vertexPosition);
    vViewNormal = vec3(uNormalMatrix * vertexNormal);
    vTexCoords = aVertexTexCoords;

    // Calcul de la position projetée
    gl_Position = uMVPMatrix * vertexPosition;
}