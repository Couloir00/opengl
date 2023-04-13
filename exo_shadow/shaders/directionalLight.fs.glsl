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
uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

//variable de sortie
out vec4 fFragColor;

//for shadow only
in vec4 vLightSpacePos;
uniform sampler2D vShadowMap;

vec3 blinnPhong(){
    vec3 wi= normalize(uLightDir_vs);
    vec3 Li= uLightIntensity;
    vec3 w0 = (normalize(-vViewPosition));
    vec3 N = vViewNormal;
    vec3 halfVector = (w0+wi)/2.f;
    return Li*(uKd*(dot(wi, N))+uKs*pow(dot(halfVector,N),uShininess));
}

float CalcShadowFactor(){

    //ClipSpace LightMVP * Position
    //NDC SCPACE = Clipsace / W
    //texture space = NDC Space *0.5+0.5

    // This transfers the vector to NDC space.
    vec3 ProjCoords=vLightSpacePos.xyz/vLightSpacePos.w;

    //prepare texture vector
    vec2 UVCoords;
    //initialize texture vector
    UVCoords.x =0.5*ProjCoords.x+0.5;
    UVCoords.y =0.5*ProjCoords.y+0.5;
    float z = 0.5 * ProjCoords.z +0.5;
    float Depth = texture(vShadowMap, UVCoords).x;

    float bias = 0.0025;

    if(Depth + bias < z)
        return 0.5;
    else
        return 1.0;

}

void main() {
    fFragColor = vec4(blinnPhong(),1.);
};