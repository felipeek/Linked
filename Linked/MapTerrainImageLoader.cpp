#include "MapTerrainImageLoader.h"
#include <string>

using namespace glm;

MapTerrainImageLoader::MapTerrainImageLoader(std::string& filename, int nChannels) : ImageLoader(filename, nChannels)
{

}


MapTerrainImageLoader::~MapTerrainImageLoader()
{

}

MapTerrain MapTerrainImageLoader::getMapTerrain(vec3 coordinateVector)
{
	MapTerrain mapObject;

	try
	{
		vec3 rgb = getPixel((int)coordinateVector.x, (int)coordinateVector.y);
		mapObject = transformRgbIntoMapTerrain(rgb);
	}
	catch (PixelOutOfBoundsException e)
	{
		mapObject = BLOCKED;
	}

	return mapObject;
}

MapTerrain MapTerrainImageLoader::transformRgbIntoMapTerrain(vec3 rgb)
{
	/* TRANSFORMATION MAP DEFINITION */

	if (rgb.r < 62 && rgb.g < 62 && rgb.b < 62)
		return BLOCKED;

	if (rgb == vec3(0, 0, 0))
		return BLOCKED;
	if (rgb.r > rgb.g && rgb.r > rgb.b)
		return DIRT;
	else if (rgb.b > rgb.g)
		return NORMAL_FLOOR;
	else
		return WATER;
	
	return MapCoordinate::STANDARD_MAP_TERRAIN;

	/* ***************************** */
}