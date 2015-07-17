#pragma once
#include "ImageLoader.h"
#include "GameEntityFactory.h"

class MapGameEntity{
public:
	MapGameEntity();
	~MapGameEntity();
	GameEntity* gameEntity;
	bool gameEntityExists;
	static MapGameEntity initWithNoGameEntity();
};

class MapGameEntityImageLoader : public ImageLoader
{
public:
	MapGameEntityImageLoader(std::string& filename, int nChannels, GameEntityFactory* gameEntityFactory);
	~MapGameEntityImageLoader();
	MapGameEntity getMapEntity(vec3 coordinateVector);
private:
	GameEntityFactory *gameEntityFactory;
};