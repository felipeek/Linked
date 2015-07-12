#include "MapTerrainImageLoader.h"
#include <string>
#include <iostream>
#define COLLISION_FACTOR 2

using namespace glm;

MapTerrainImageLoader::MapTerrainImageLoader(std::string& filename, int nChannels) : ImageLoader(filename, nChannels)
{

}


MapTerrainImageLoader::~MapTerrainImageLoader()
{

}

MapTerrain MapTerrainImageLoader::getMapTerrainForPlayerMovement(vec3 coordinateVector){
	MapTerrain mapObject, aux;

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
					if (MapCoordinate::isOfCollisionType(transformRgbIntoMapTerrain(rgb)))
						return transformRgbIntoMapTerrain(rgb);
				}
	}
	catch (PixelOutOfBoundsException e)
	{
		mapObject = BLOCKED;
	}

	return mapObject;
}

MapTerrain MapTerrainImageLoader::getMapTerrainForMapCreation(vec3 coordinateVector)
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
	else if (rgb.r > rgb.g && rgb.r > rgb.b)
		return DIRT;
	else if (rgb.b > rgb.g)
		return NORMAL_FLOOR;
	else
		return WATER;

	return MapCoordinate::STANDARD_MAP_TERRAIN;

	/* ***************************** */
}