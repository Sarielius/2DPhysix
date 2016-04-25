#pragma once
#include <SFML\Graphics.hpp>

class Object
{
public:
	Object(sf::Vector2f dimensions)
	{
		shape.setSize(dimensions);
	};


private:
	sf::RectangleShape shape;
};

