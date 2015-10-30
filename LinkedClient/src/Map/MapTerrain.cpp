#include "MapTerrain.h"

using namespace glm;

MapTerrainImageLoader::MapTerrainImageLoader(std::string& filename, int nChannels) : ImageLoader(filename, nChannels)
{

}

MapTerrainImageLoader::~MapTerrainImageLoader()
{
	freeLoadedImage();
}

bool MapTerrainImageLoader::coordinateHasCollision(glm::vec3 coordinateVector){
	try
	{
		for (int i = 1; i <= COLLISION_FACTOR; i++)
			for (int j = i; j >= -i; j--)
				for (int k = i; k >= -i; k--)
				{
					glm::vec3 rgb = getPixel((int)coordinateVector.x + j, (int)coordinateVector.y + k);
					MapTerrain mapTerrain = transformRgbIntoMapTerrain(rgb);
					if (mapTerrain == BLOCKED || mapTerrain == WATER)
						return true;
				}
	}
	catch (PixelOutOfBoundsException e)
	{
		return false;
	}

	return false;
}

MapTerrain MapTerrainImageLoader::getMapTerrainForCoordinate(vec3 coordinateVector)
{
	MapTerrain mapObject;

	try
	{
		vec3 rgb = getPixel((int)floor(coordinateVector.x), (int)floor(coordinateVector.y));
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