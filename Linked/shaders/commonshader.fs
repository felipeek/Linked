#version 330 core

in vec2 uvCoords;
in vec3 normal;
in vec3 surfaceToLight;
in vec3 lightPos;
in vec3 lightCol;

out vec4 out_Color;

uniform sampler2D TextureSampler;

void main(){
	
	vec4 textureColor = texture(TextureSampler, uvCoords);
	if(textureColor.a < 0.8)
	{
		discard;
	}
	
	//vec3 surfaceToLight2 = vec3(100,100,100);
	float cosAngle = dot(normalize(normal), normalize(surfaceToLight));
	vec3 diffuse = max(cosAngle * lightCol, 0.2);
	
	out_Color = textureColor * vec4(diffuse, 1.0);
}