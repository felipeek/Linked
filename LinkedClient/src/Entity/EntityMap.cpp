#include "EntityMap.h"
#include "MapShader.h"

EntityMap::EntityMap(Transform* transform, Mesh* mesh, Texture* texture, Texture* texture1, Texture* texture2, Texture* texture3, Texture* blendMap) : Entity(transform, mesh, texture)
{
	this->texture1 = texture1;
	this->texture2 = texture2;
	this->texture3 = texture3;
	this->blendMap = blendMap;
}

EntityMap::~EntityMap()
{
	if (mesh != nullptr)
		delete mesh;
	if (texture != nullptr)
		delete texture;
	if (texture1 != nullptr)
		delete texture1;
	if (texture2 != nullptr)
		delete texture2;
	if (texture3 != nullptr)
		delete texture3;
	if (blendMap != nullptr)
		delete blendMap;
}

void EntityMap::render(MapShader* shader, Camera* lightCamera)
{
	shader->setLightCamera(lightCamera);
	Entity::render(shader);
}

void EntityMap::setShadowTexture(Texture* shadowTexture)
{
	this->shadowTexture = shadowTexture;
}

void EntityMap::bindTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->textureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture1->textureID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture2->textureID);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture3->textureID);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, blendMap->textureID);

	// bind Shadow Map
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, shadowTexture->textureID);
	
}
