#version 330

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 textureCoord;

out vec2 uvCoords;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;
out vec4 fragPosLightSpace;
out vec3 second_light_pos;

uniform mat4 Model;
uniform mat4 viewProj;
uniform mat4 lightSpaceMatrix;
uniform vec3 other_light_pos;

void main()
{
	vec4 worldPosition = Model * vec4(vertexPosition_modelspace, 1.0);
	gl_Position = viewProj * worldPosition;
	
	uvCoords = vec2(textureCoord.x, textureCoord.y);
	texCoords = uvCoords;
	normal = vec3(Model * vec4(vertexNormal_modelspace, 1.0));
	fragPos = vec3(worldPosition);
	fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
	second_light_pos = other_light_pos;
}