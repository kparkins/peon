#version 330 core

uniform sampler2D objectTexture;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

in vec3 vFragmentPosition;
in vec3 vFragmentNormal;
in vec2 vTextureCoordinate;

out vec4 fragmentColor;

void main() {
    vec3 lightDirection = normalize(lightPosition - vFragmentPosition);

    float ambientCoefficient = 0.1;
    vec3 ambient = ambientCoefficient * lightColor;

    float diffuseCoefficient = max(dot(vFragmentNormal, lightDirection), 0.0);
    vec3 diffuse = diffuseCoefficient * lightColor;

    float specularCoefficient = 0.5;
    vec3 viewDirection = normalize(viewPosition - vFragmentPosition);
    vec3 halfwayDirection = normalize(viewDirection + lightDirection);
    vec3 specular = specularCoefficient * pow(max(dot(halfwayDirection, vFragmentNormal), 0.0), 32) * lightColor;

    vec3 frag = pow((ambient + diffuse + specular), vec3(1.0/2.2)); 
    fragmentColor = vec4(frag * vec3(texture(objectTexture, vTextureCoordinate)), 1.0);
}