#pragma once
#include "ImageLoader.h"

enum MapEntity{
	NONE,
	FIRE,
	MUD
};

class MapEntityImageLoader : public ImageLoader
{
public:
	MapEntityImageLoader(std::string& filename, int nChannels);
	~MapEntityImageLoader();
	MapEntity getMapEntity(vec3 coordinate);
private:
	MapEntity transformRgbIntoMapEntity(vec3 rgb);
};