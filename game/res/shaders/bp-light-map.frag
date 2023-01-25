#version 330 core

struct PhongLightMap {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

uniform sampler2D objectTexture;
uniform PhongLightMap object;
uniform Light light;
uniform vec3 viewPosition;

in vec3 vFragmentPosition;
in vec3 vFragmentNormal;
in vec2 vTextureCoordinate;

out vec4 fragmentColor;

void main() {
    vec3 lightDirection = normalize(light.position - vFragmentPosition);

    vec3 ambient = light.ambient * vec3(texture(object.diffuse, vTextureCoordinate));

    float diffuseCoefficient = max(dot(vFragmentNormal, lightDirection), 0.0);
    vec3 diffuse = diffuseCoefficient * light.diffuse * vec3(texture(object.diffuse, vTextureCoordinate));

    vec3 viewDirection = normalize(viewPosition - vFragmentPosition);
    vec3 halfwayDirection = normalize(viewDirection + lightDirection);
    float specularCoefficient = pow(max(dot(halfwayDirection, vFragmentNormal), 0.0), object.shininess);
    vec3 specular = specularCoefficient * vec3(texture(object.specular, vTextureCoordinate)) * light.specular;

    fragmentColor = vec4((ambient + diffuse + specular) , 1.0);
}