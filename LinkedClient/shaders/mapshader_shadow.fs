#version 330 core

in vec2 uvCoords;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;
in vec4 fragPosLightSpace;

out vec4 out_Color;

uniform sampler2D NormalFloor;
uniform sampler2D Blocked;
uniform sampler2D Water;
uniform sampler2D Dirt;
uniform sampler2D BlendMap;

uniform sampler2D shadowMap;
uniform vec3 lightPosition;
uniform vec3 lightIntensity;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias =  max(0.05*(1.0 - dot(normalize(normal), lightPosition - fragPos)), 0.005);
	
	float shadow = 0.0;
	
	vec2 texelSize = 1.0 / textureSize(shadowMap,0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 0.7 : 0.1;
		}
	}
	shadow /= 9.0;
	//shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	
	if(projCoords.z > 1.0)
		shadow = 0.0;
	
	return shadow;
}

void main(){
	vec3 surfaceToLight = lightPosition - fragPos;
	vec3 lightColor = lightIntensity;
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
	vec2 tiledCoords = uvCoords * 20.0;
	vec4 backgroundTextureColor = texture(Blocked, tiledCoords*1.5) * backTextureAmount;
	vec4 rTextureColor = texture(Dirt, tiledCoords*4) * blendMapColor.r;
	vec4 gTextureColor = texture(Water, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(NormalFloor, tiledCoords*4) * blendMapColor.b;

	vec4 totalColor;

	//if (blendMapColor.r < 0.35 && blendMapColor.g < 0.35 && blendMapColor.b < 0.35)
	//	totalColor = backgroundTextureColor;
	//else
	totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
		
	vec3 Normal = normalize(normal);
	vec3 ambient = 0.15 * lightIntensity;
	vec3 lightDir = normalize(surfaceToLight);
	float diff = max(dot(lightDir, Normal), 0.0);
	vec3 diffuse = diff * lightIntensity;
	// Shadow
	float shadow = ShadowCalculation(fragPosLightSpace);
	vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * totalColor.rgb;
	out_Color = vec4(lighting, 1.0);
}