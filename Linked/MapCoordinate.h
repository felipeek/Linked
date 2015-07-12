enum MapTerrain {
	NORMAL_FLOOR,
	BLOCKED,
	WATER,
	DIRT
};

enum MapEntity{
	NONE,
	FIRE,
	MUD
};

class MapCoordinate {
public:
	MapCoordinate();
	~MapCoordinate();
	MapTerrain terrain;
	MapEntity entity;
	static const MapTerrain STANDARD_MAP_TERRAIN = NORMAL_FLOOR;
	static const MapEntity STANDARD_MAP_ENTITY = NONE;
	static bool isOfCollisionType(MapTerrain terrain){
		if (terrain == BLOCKED || terrain == WATER)
			return true;
		return false;
	}
};