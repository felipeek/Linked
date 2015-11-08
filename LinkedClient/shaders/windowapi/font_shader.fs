#version 330 core

in vec2 uvCoords;
in vec4 position;

out vec4 out_Color;

uniform sampler2D textTexture;
uniform vec4 textColor;
uniform vec2 clipTL;
uniform vec2 clipBR;

void main(){
	if(position.x < clipTL.x || position.x > clipBR.x)
		discard;
	if(position.y > -clipTL.y || position.y < -clipBR.y)
		discard;
	vec4 textureColor = texture(textTexture, uvCoords);
	vec4 finalColor = vec4(textColor.rgb, textureColor.r + textColor.a - 1.0);
	
	vec4 alpha = vec4(finalColor.a, finalColor.a, finalColor.a, 1.0);
	out_Color = finalColor;
}