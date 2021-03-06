#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 normalizedNormal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    //diffuse
    float diff = max(dot(normalizedNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normalizedNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = ( ambient + diffuse + specular ) * objectColor;

    FragColor = vec4(result, 1.0); 
};