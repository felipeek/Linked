#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 textureCoord;

out vec2 uvCoords;
out vec3 normal;
out vec3 lightPos;
out vec3 lightCol;
out vec3 surfaceToLight;

uniform mat4 Model;
uniform mat4 viewProj;
uniform float textureNumRows;
uniform vec2 textureOffset;

uniform vec3 lightPosition;
uniform vec3 lightColor;

void main()
{
	vec4 worldPosition = Model * vec4(vertexPosition_modelspace, 1.0);
	gl_Position = viewProj * worldPosition;
	uvCoords = vec2(textureCoord.x, textureCoord.y);
	uvCoords = (uvCoords/textureNumRows) + textureOffset;
	
	normal = vec3(Model * vec4(vertexNormal_modelspace, 1.0));
	
	lightPos = lightPosition;
	lightCol = lightColor;
	surfaceToLight = lightPos - worldPosition.xyz;
}