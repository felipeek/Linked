#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;

out vec2 uvCoords;
out vec4 position;

uniform mat4 Model;

void main()
{
	gl_Position = Model * vec4(vertexPosition_modelspace, 1.0);
	uvCoords = texCoords;
	position = gl_Position;
}