#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 textureCoord;

out vec2 uvCoords;
out vec3 normal;
out vec3 lightPos;
out vec3 lightCol;
out vec3 surfaceToLight;
out vec3 second_light_pos;
out vec3 fragPos;

uniform mat4 Model;
uniform mat4 viewProj;
uniform float textureNumRows;
uniform vec2 textureOffset;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 other_light_pos;

void main()
{
	vec4 worldPosition = Model * vec4(vertexPosition_modelspace, 1.0);
	fragPos = vec3(worldPosition);
	gl_Position = viewProj * worldPosition;
	uvCoords = vec2(textureCoord.x, textureCoord.y);
	uvCoords = (uvCoords/textureNumRows) + textureOffset;
	
	normal = vec3(Model * vec4(vertexNormal_modelspace, 0.0));
	
	lightPos = lightPosition;
	lightCol = lightColor;
	surfaceToLight = lightPos - worldPosition.xyz;
	second_light_pos = other_light_pos;
}