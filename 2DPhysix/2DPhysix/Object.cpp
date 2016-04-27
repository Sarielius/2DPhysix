#include "Object.h"

void Object::render(sf::RenderWindow& win)
{
	win.draw(shape);
}

void Object::update(float deltaTime)
{
	// All constant physics stuff in here, transforms for the shape etc. 
	// Collisions likely handled elsewhere, data then moved here for processing.
	// No friction, horizontal acceleration is ignored for now.

	if (movable && rotatable)
	{
		// Standard falling motion with x-directional velocity.
		posY = posY + vy * deltaTime;
		vy = vy - g * deltaTime;
		posX = posX + vx * deltaTime;
	}

	

	shape.setPosition(posX, posY);
	// Might get huge
}

