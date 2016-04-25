#pragma once
#include "Object.h"
#include <vector>

class Overlord
{
public:
	Overlord();
	~Overlord();
private:
	std::vector<sf::RectangleShape> shapes;
};

