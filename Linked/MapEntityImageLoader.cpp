#include "MapEntityImageLoader.h"

using namespace glm;

MapEntityImageLoader::MapEntityImageLoader(std::string& filename, int nChannels) : ImageLoader(filename, nChannels)
{
}


MapEntityImageLoader::~MapEntityImageLoader()
{
}

MapEntity MapEntityImageLoader::getMapEntity(vec3 coordinateVector)
{
	MapEntity mapEntity;

	try
	{
		vec3 rgb = getPixel((int)coordinateVector.x, (int)coordinateVector.y);
		mapEntity = transformRgbIntoMapEntity(rgb);
	}
	catch (PixelOutOfBoundsException e)
	{
		mapEntity = NONE;
	}

	return mapEntity;
}

MapEntity MapEntityImageLoader::transformRgbIntoMapEntity(vec3 rgb)
{
	/* TRANSFORMATION MAP DEFINITION */

	if (rgb == vec3(255, 0, 0))
		return NONE;
	if (rgb == vec3(0, 255, 0))
		return FIRE;
	if (rgb == vec3(0, 0, 255))
		return MUD;

	return MapCoordinate::STANDARD_MAP_ENTITY;

	/* ***************************** */
}