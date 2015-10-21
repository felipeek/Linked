#pragma once
#include "Entity.h"
class MapShader;

class EntityMap : public Entity
{
public:
	EntityMap(Transform* transform, Mesh* mesh, Texture*, Texture*, Texture*, Texture*, Texture*);
	~EntityMap();
	virtual void render(MapShader* shader);
	virtual void bindTextures();
	Texture* texture1;
	Texture* texture2;
	Texture* texture3;
	Texture* blendMap;
};
