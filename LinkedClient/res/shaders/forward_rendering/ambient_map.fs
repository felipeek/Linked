#version 330 core

in vec2 uvCoords;

out vec4 out_Color;

uniform sampler2D NormalFloor;
uniform sampler2D Blocked;
uniform sampler2D Water;
uniform sampler2D Dirt;
uniform sampler2D BlendMap;


void main(){
	
	vec4 blendMapColor = texture(BlendMap, uvCoords);
	
	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = uvCoords * 40.0;
	vec4 backgroundTextureColor = texture(Blocked, tiledCoords*1.5) * backTextureAmount;
	vec4 rTextureColor = texture(Dirt, tiledCoords*4) * blendMapColor.r;
	vec4 gTextureColor = texture(Water, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(NormalFloor, tiledCoords*4) * blendMapColor.b;

	vec4 totalColor;

	totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
		
	out_Color = totalColor * vec4(0.1, 0.1, 0.1, 1);
}