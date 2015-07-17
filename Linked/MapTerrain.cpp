#include "MapTerrain.h"
#include <string>

using namespace glm;

MapTerrainImageLoader::MapTerrainImageLoader(std::string& filename, int nChannels) : ImageLoader(filename, nChannels)
{

}


MapTerrainImageLoader::~MapTerrainImageLoader()
{

}

MapTerrain MapTerrainImageLoader::getMapTerrainWithMovementCollisionForCoordinate(vec3 coordinateVector){
	MapTerrain mapObject;

	try
	{
		vec3 rgb = getPixel((int)coordinateVector.x, (int)coordinateVector.y);
		mapObject = transformRgbIntoMapTerrain(rgb);
		rgb = getPixel((int)coordinateVector.x+1, (int)coordinateVector.y);

		for (int i = 1; i <= COLLISION_FACTOR; i++)
			for (int j = i; j >= -i; j--)
				for (int k = i; k >= -i; k--)
				{
					rgb = getPixel((int)coordinateVector.x + j, (int)coordinateVector.y + k);
					if (isOfCollisionType(transformRgbIntoMapTerrain(rgb)))
						return transformRgbIntoMapTerrain(rgb);
				}
	}
	catch (PixelOutOfBoundsException e)
	{
		mapObject = BLOCKED;
	}

	return mapObject;
}

MapTerrain MapTerrainImageLoader::getMapTerrainForCoordinate(vec3 coordinateVector)
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

	if (rgb.r < THRESHOLD && rgb.g < THRESHOLD && rgb.b < THRESHOLD)
		return BLOCKED;
	else if (rgb.r > rgb.g && rgb.r > rgb.b)
		return DIRT;
	else if (rgb.b > rgb.g)
		return NORMAL_FLOOR;
	else
		return WATER;

	return BLOCKED;

	/* ***************************** */
}

bool MapTerrainImageLoader::isOfCollisionType(MapTerrain terrain)
{
	if (terrain == BLOCKED || terrain == WATER)
		return true;
	return false;
}