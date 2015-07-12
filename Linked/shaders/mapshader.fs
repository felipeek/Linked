#version 330 core

in vec2 uvCoords;

out vec4 out_Color;

uniform sampler2D NormalFloor;
uniform sampler2D Blocked;
uniform sampler2D Water;
uniform sampler2D BlendMap;


void main(){
	
	//vec4 blendMapColor = texture(BlendMap, uvCoords);
	
	//vec4 tNormalFloor = texture(NormalFloor, uvCoords * 40.0);
	//vec4 tBlocked = texture(Blocked, uvCoords * 40.0);
	//vec4 tWater = texture(Water, uvCoords * 40.0);

	//float perc_NormalFloor = clamp(1 - length(blendMapColor - vec4(1,1,1,1)), 0, 1);
	//float perc_Blocked = clamp(1 - length(blendMapColor - vec4(0,0,0,1)), 0, 1);
	//float perc_Water = clamp(1 - length(blendMapColor - vec4(0,0,1,1)), 0, 1);
	
	//out_Color = tNormalFloor * perc_NormalFloor + tBlocked * perc_Blocked + tWater * perc_Water;
	vec4 blendMapColor = texture(BlendMap, uvCoords);
	
	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = uvCoords * 40.0;
	vec4 backgroundTextureColor = texture(NormalFloor, tiledCoords) * backTextureAmount;
	vec4 rTextureColor = texture(NormalFloor, tiledCoords) * blendMapColor.r;
	vec4 gTextureColor = texture(Blocked, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(Water, tiledCoords) * blendMapColor.b;
	
	vec4 totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
	out_Color = totalColor;
}