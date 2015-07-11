#include "MapObjectImageLoader.h"
#include <string>

using namespace glm;

MapObjectImageLoader::MapObjectImageLoader(std::string& filename, int nChannels) : ImageLoader(filename, nChannels)
{

}


MapObjectImageLoader::~MapObjectImageLoader()
{

}

MapObject MapObjectImageLoader::getMapObject(vec3 coordinateVector)
{
	MapObject mapObject;

	try
	{
		vec3 rgb = getPixel((int)coordinateVector.x, (int)coordinateVector.y);
		mapObject = transformRgbIntoMapObject(rgb);
	}
	catch (PixelOutOfBoundsException e)
	{
		mapObject = BLOCKED;
	}

	return mapObject;
}

MapObject MapObjectImageLoader::transformRgbIntoMapObject(vec3 rgb)
{
	/* TRANSFORMATION MAP DEFINITION */

	if (rgb == vec3(255, 255, 255))
		return NORMAL_FLOOR;
	if (rgb == vec3(0, 0, 0))
		return BLOCKED;
	if (rgb == vec3(0, 0, 255))
		return HOLE;
	if (rgb == vec3(0, 255, 0))
		return SPIKES;
	if (rgb == vec3(255, 0, 0))
		return FIRE;
	if (rgb == vec3(0, 255, 255))
		return MUD;
	if (rgb == vec3(255, 0, 255))
		return SLIPPERY;
	
	return MapCoordinate::STANDARD_MAP_OBJECT;

	/* ***************************** */
}