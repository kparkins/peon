#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

uniform sampler2D objectTexture;
uniform Material objectMaterial;
uniform Light light;
uniform vec3 viewPosition;

in vec3 vFragmentPosition;
in vec3 vFragmentNormal;
in vec2 vTextureCoordinate;

out vec4 fragmentColor;

void main() {
    vec3 lightDirection = normalize(light.position - vFragmentPosition);

    vec3 ambient = objectMaterial.ambient * light.ambient;

    float diffuseCoefficient = max(dot(vFragmentNormal, lightDirection), 0.0);
    vec3 diffuse = diffuseCoefficient * light.diffuse * objectMaterial.diffuse;

    vec3 viewDirection = normalize(viewPosition - vFragmentPosition);
    vec3 halfwayDirection = normalize(viewDirection + lightDirection);
    float specularCoefficient = pow(max(dot(halfwayDirection, vFragmentNormal), 0.0), objectMaterial.shininess);
    vec3 specular = specularCoefficient * objectMaterial.specular * light.specular;

    vec3 frag = pow((ambient + diffuse + specular), vec3(1.0/2.2));
    fragmentColor = vec4(frag, 1.0);
}