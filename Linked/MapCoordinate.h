enum MapObject {
	NORMAL_FLOOR,
	BLOCKED,
	HOLE,
	SPIKES,
	FIRE,
	MUD,
	SLIPPERY,
};

enum MapHeight{
	GROUND,
	FIRST_FLOOR,
	SECOND_FLOOR
};

class MapCoordinate {
public:
	MapCoordinate();
	~MapCoordinate();
	MapObject object;
	MapHeight height;
	static const MapObject STANDARD_MAP_OBJECT = NORMAL_FLOOR;
	static const MapHeight STANDARD_MAP_HEIGHT = GROUND;
};