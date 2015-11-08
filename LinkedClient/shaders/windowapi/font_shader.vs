#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

out vec2 uvCoords;
out vec4 position;

void main()
{
	gl_Position = vec4(vertexPosition_modelspace, 1.0);
	position = gl_Position;
	uvCoords = texCoords;
}