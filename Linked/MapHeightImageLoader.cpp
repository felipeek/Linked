#include "MapHeightImageLoader.h"

using namespace glm;

MapHeightImageLoader::MapHeightImageLoader(std::string& filename, int nChannels) : ImageLoader(filename, nChannels)
{
}


MapHeightImageLoader::~MapHeightImageLoader()
{
}

MapHeight MapHeightImageLoader::getMapHeight(vec3 coordinateVector)
{
	MapHeight mapHeight;

	try
	{
		vec3 rgb = getPixel((int)coordinateVector.x, (int)coordinateVector.y);
		mapHeight = transformRgbIntoMapHeight(rgb);
	}
	catch (PixelOutOfBoundsException e)
	{
		mapHeight = GROUND;
	}

	return mapHeight;
}

MapHeight MapHeightImageLoader::transformRgbIntoMapHeight(vec3 rgb)
{
	/* TRANSFORMATION MAP DEFINITION */

	if (rgb == vec3(255, 0, 0))
		return GROUND;
	if (rgb == vec3(0, 255, 0))
		return FIRST_FLOOR;
	if (rgb == vec3(0, 0, 255))
		return SECOND_FLOOR;

	return MapCoordinate::STANDARD_MAP_HEIGHT;

	/* ***************************** */
}