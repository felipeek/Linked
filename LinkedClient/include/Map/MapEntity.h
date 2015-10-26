#pragma once
#include "ImageLoader.h"
#include "GameEntityFactory.h"

#define DEFAULT_ENTITIES_MAP_COLOR_RED 0
#define DEFAULT_ENTITIES_MAP_COLOR_GREEN 0
#define DEFAULT_ENTITIES_MAP_COLOR_BLUE 0

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
	virtual ~MapGameEntityImageLoader();
	MapGameEntity getMapEntity(vec3 coordinateVector);
	bool coordinateHasCollision(vec3 coordinate);
private:
	GameEntityFactory *gameEntityFactory;
};