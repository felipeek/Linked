#version 330 core

in vec2 uvCoords;

out vec4 out_Color;

uniform sampler2D TextureSampler;

void main(){
	
	vec4 textureColor = texture(TextureSampler, uvCoords);
	if(textureColor.a < 0.8)
	{
		discard;
	}
	
	out_Color = textureColor;
}