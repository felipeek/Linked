#version 330 core

in vec2 uvCoords;
in vec3 normal;
in vec3 surfaceToLight;
in vec3 lightPos;
in vec3 lightColor;

out vec4 out_Color;

uniform sampler2D NormalFloor;
uniform sampler2D Blocked;
uniform sampler2D Water;
uniform sampler2D Dirt;
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
	vec4 backgroundTextureColor = texture(Blocked, tiledCoords*1.5) * backTextureAmount;
	vec4 rTextureColor = texture(Dirt, tiledCoords*4) * blendMapColor.r;
	vec4 gTextureColor = texture(Water, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(NormalFloor, tiledCoords*4) * blendMapColor.b;

	vec4 totalColor;

	//if (blendMapColor.r < 0.35 && blendMapColor.g < 0.35 && blendMapColor.b < 0.35)
	//	totalColor = backgroundTextureColor;
	//else
		totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
		
	vec3 lightInt = vec3(1,1,1);
	
	float cosDiffuse = dot(normalize(normal), normalize(surfaceToLight));
	vec3 diffuse = max(cosDiffuse * lightColor, 0.2);

	out_Color = totalColor * vec4(diffuse, 1);
}