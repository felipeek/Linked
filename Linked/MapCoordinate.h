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
	static const MapObject STANDARD_MAP_OBJECT = NORMAL_FLOOR;
	static const MapHeight STANDARD_MAP_HEIGHT = GROUND;
	MapCoordinate();
	~MapCoordinate();
	MapObject object;
	MapHeight height;
};