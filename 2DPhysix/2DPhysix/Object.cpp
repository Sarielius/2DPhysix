#include "Object.h"


Object::Object(float X, float Y, float positionX, float positionY, float mass, bool mov, bool rot, float gravity) :
	g(gravity),
	movable(mov),
	rotatable(rot),
	vx(0.0f),
	vy(0.0f),
	posX(positionX),
	posY(positionY),
	mass(mass),
	angle(0.0f),
	angVel(0.0f),
	forceMultiplier(100.0f)
{
	shape.setSize(sf::Vector2f(X, Y));
	shape.setOrigin(X / 2, Y / 2); // Origin also acts as center of mass. getOrigin() for access.

	for (size_t i = 0; i < 4; i++) // Push our points into a vector
	{
		points.push_back(shape.getPoint(i));
	}

	// Hard coded, might change later, might not.

	sf::Vector2f edge = points[0] - points[1];
	sf::Vector2f normal(edge.x, -edge.y);
	axis.push_back(normal);

	edge = points[1] - points[2];
	normal = sf::Vector2f(edge.x, -edge.y);
	axis.push_back(normal);

	edge = points[2] - points[3];
	normal = sf::Vector2f(edge.x, -edge.y);
	axis.push_back(normal);

	edge = points[3] - points[0];
	normal = sf::Vector2f(edge.x, -edge.y);
	axis.push_back(normal);
}


void Object::render(sf::RenderWindow& win)
{
	win.draw(shape);
}

void Object::updateAxis()
{
	// Hard code is best code

	sf::Vector2f edge = points[0] - points[1];
	sf::Vector2f normal(edge.x, -edge.y);
	axis[0] = normal;

	edge = points[1] - points[2];
	normal = sf::Vector2f(edge.x, -edge.y);
	axis[1] = normal;

	edge = points[2] - points[3];
	normal = sf::Vector2f(edge.x, -edge.y);
	axis[2] = normal;

	edge = points[3] - points[0];
	normal = sf::Vector2f(edge.x, -edge.y);
	axis[3] = normal;
}

void Object::update(float deltaTime)
{
	// All constant physics stuff in here, transforms for the shape etc. 
	// Collisions likely handled elsewhere (or not :O), data then moved here for processing.
	// No friction, horizontal acceleration is ignored for now.
	// Rotation ignores slowing elements.

	if (movable) // X and Y positions change.
	{
		// Standard falling motion with x-directional velocity.
		posY = posY + vy * deltaTime;
		vy = vy - g * deltaTime;
		posX = posX + vx * deltaTime;
	}

	if (rotatable) // Object can rotate.
	{
		angle = angle + angVel * deltaTime;
	}
	
	shape.setRotation(angle);
	shape.setPosition(posX, posY);

	updateAxis(); // Get all axes for this object for SAT.

	// Might get huge
}

