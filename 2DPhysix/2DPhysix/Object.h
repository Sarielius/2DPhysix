#pragma once
#include <SFML\Graphics.hpp>

class Object
{
public:
	Object(float X, float Y, float positionX, float positionY, bool mov, bool rot, float gravity) :
		g(gravity),
		movable(mov),
		rotatable(rot),
		vx(0.0f),
		vy(0.0f)
	{
		shape.setSize(sf::Vector2f(X, Y));
		posX = positionX;
		posY = positionY;
	};

	sf::RectangleShape& getShape()
	{
		return shape;
	}

	void setHorizontalVelocity(float vel)
	{
		vx = vel;
	}

	void setVerticalVelocity(float vel)
	{
		vy = vel;
	}

	void update(float deltaTime);
	void render(sf::RenderWindow& win);

private:
	sf::RectangleShape shape;
	bool movable; // Can the object be moved by anything
	bool rotatable; // 
	float g; // Gravity
	float vx; // Horizontal speed
	float vy; // Vertical speed
	float posX; // Horizontal location.
	float posY; // Vertical location.
};

