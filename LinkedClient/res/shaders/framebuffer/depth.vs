#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

out vec2 TextureCoords;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f); 
    TextureCoords = texCoords;
}  