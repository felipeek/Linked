#pragma once
#include "Entity.h"
class MapShader;
class Camera;

class EntityMap : public Entity
{
public:
	EntityMap(Transform* transform, Mesh* mesh, Texture*, Texture*, Texture*, Texture*, Texture*);
	~EntityMap();
	virtual void render(MapShader* shader, Camera* lightCamera);
	virtual void bindTextures();
	void setShadowTexture(Texture* shadowTexture);
	void setLightCamera(Camera* c);

	Texture* texture1;
	Texture* texture2;
	Texture* texture3;
	Texture* blendMap;
	Texture* shadowTexture;
};

