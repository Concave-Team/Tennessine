#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
    TexCoord = aTexCoord;
    Normal = aNormal;
    FragPos = vec3(Model * vec4(aPos, 1.0));
    gl_Position = Projection * View * Model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}