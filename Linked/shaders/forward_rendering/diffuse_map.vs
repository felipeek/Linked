#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 textureCoord;

out vec2 uvCoords;
out vec3 normal;
out vec3 lightPos;
out vec3 lightColor;
out vec3 surfaceToLight;
out vec3 vertPos;

uniform mat4 Model;
uniform mat4 viewProj;
uniform vec3 lightPosition;
uniform vec3 lightIntensity;

void main()
{
	vec4 worldPosition = Model * vec4(vertexPosition_modelspace, 1.0);
	gl_Position = viewProj * worldPosition;
	uvCoords = vec2(textureCoord.x, textureCoord.y);
	normal = vec3(Model * vec4(vertexNormal_modelspace, 1.0));
	
	lightPos = lightPosition;
	lightColor = lightIntensity;
	surfaceToLight = lightPos - worldPosition.xyz;
	vertPos = worldPosition.xyz;
}