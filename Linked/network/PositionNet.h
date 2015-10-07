#pragma once

#include "../include/glm/glm.hpp"

class PositionNet
{
public:
	PositionNet(int id, int xid, glm::vec3& position);
	~PositionNet();
private:
	glm::vec3 position;
};