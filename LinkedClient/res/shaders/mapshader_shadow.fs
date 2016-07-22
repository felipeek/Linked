#version 330 core

in vec2 uvCoords;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;
in vec4 fragPosLightSpace;
in vec3 second_light_pos;

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
	float bias =  max(0.05*(1.0 - dot(normalize(normal), lightPosition - fragPos)), 0.003);
	
	float shadow = 0.0;
	
	vec2 texelSize = 1.0 / textureSize(shadowMap,0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 0.9 : 0.2;
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

	vec4 blendMapColor = texture(BlendMap, uvCoords);
	
	float backTextureAmount = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);
	vec2 tiledCoords = uvCoords * 20.0;
	vec4 backgroundTextureColor = texture(Blocked, tiledCoords*1.5) * backTextureAmount;
	vec4 rTextureColor = texture(Dirt, tiledCoords*4) * blendMapColor.r;
	vec4 gTextureColor = texture(Water, tiledCoords) * blendMapColor.g;
	vec4 bTextureColor = texture(NormalFloor, tiledCoords*4) * blendMapColor.b;

	vec4 totalColor;

	totalColor = backgroundTextureColor + rTextureColor + gTextureColor + bTextureColor;
		
	vec3 Normal = normalize(normal);
	vec3 ambient = 0.15 * lightIntensity;
	vec3 lightDir = normalize(surfaceToLight);
	float diff = max(dot(lightDir, Normal), 0.0);
	vec3 diffuse = diff * lightIntensity;
	// Shadow
	float shadow = ShadowCalculation(fragPosLightSpace);
	shadow = 0.0;
	vec3 lighting = (ambient + (1.0 - shadow) * diffuse) * totalColor.rgb;
	
	vec3 seconddistance = second_light_pos - fragPos;
	
	float att = clamp(1.0 - length(seconddistance)/30.0, 0.0, 1.0);
	att = att * att;
	
	vec3 second_diffuse = max(dot(normalize(seconddistance), Normal), 0.0) * totalColor.rgb * vec3(att, att, att*0.95);

	out_Color = vec4(lighting + second_diffuse, 1.0);
}