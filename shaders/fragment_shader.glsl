#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 objectColor; // Sin valor por defecto

void main()
{
    float diff = max(dot(normalize(Normal), -lightDir), 0.0);
    vec3 color = objectColor * diff + objectColor * 0.2;
    FragColor = vec4(color, 1.0);
}