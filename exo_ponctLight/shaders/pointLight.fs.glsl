#version 410
//variables d'entrees
in vec3 vViewPosition;
in vec3 vViewNormal;
in vec2 vTexCoords;

//object materials
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

//light infos
uniform vec3 uLightPos_vs;
uniform vec3 uLightIntensity;


//variable de sortie
out vec4 fFragColor;

vec3 blinnPhong(){
    float d = distance(vViewPosition, uLightPos_vs);
    vec3 wi= normalize(uLightPos_vs-vViewPosition);
    vec3 Li= (uLightIntensity/(d*d));
    vec3 w0 = (normalize(-vViewPosition));
    vec3 N = vViewNormal;
    vec3 halfVector = (w0+wi)/2.f;
    return Li*(uKd*(dot(wi, N))+uKs*pow(dot(halfVector,N),uShininess));
}

void main() {
    fFragColor = vec4(blinnPhong(),1.);
};