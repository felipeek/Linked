#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 textureCoord;

out vec2 uvCoords;

uniform float textureNumRows;
uniform vec2 textureOffset;

uniform mat4 Model;

void main()
{
	vec4 worldPosition = Model * vec4(vertexPosition_modelspace.xy, 0.0, 1.0);
	gl_Position = worldPosition;
	uvCoords = vec2(textureCoord.x, textureCoord.y);
	uvCoords = (uvCoords/textureNumRows) + textureOffset;
}