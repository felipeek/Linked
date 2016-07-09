#version 330 core

in vec2 uvCoords;
in vec3 normal;
in vec3 surfaceToLight;
in vec3 lightPos;
in vec3 lightCol;
in vec3 second_light_pos;
in vec3 fragPos;

out vec4 out_Color;

uniform sampler2D TextureSampler;

void main(){
	
	vec4 textureColor = texture(TextureSampler, uvCoords);
	if(textureColor.a < 0.8)
	{
		discard;
	}
	
	float cosAngle = dot(normalize(normal), normalize(surfaceToLight));
	vec3 diffuse = max(cosAngle * lightCol, 0.2);
	
	vec3 seconddistance = second_light_pos - fragPos;
	float att = clamp(1.0 - length(seconddistance)/30.0, 0.0, 1.0);
	att = att * att;
	
	vec3 second_diffuse = max(dot(normalize(seconddistance), normalize(normal)), 0.0) * vec3(att, att, att*0.95);
	
	out_Color = textureColor * vec4(diffuse + second_diffuse, 1.0);
}