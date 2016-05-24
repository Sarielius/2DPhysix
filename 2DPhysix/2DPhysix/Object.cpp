#include "Object.h"
#include <iostream>

Object::Object(const float X, const float Y, const float positionX, const float positionY, const float mass, bool mov, bool rot, const float gravity) :
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
	window(nullptr)
{ 
	

	shape.setSize(sf::Vector2f(X, Y));

	size.x = X;
	size.y = Y;

	shape.setOrigin(X / 2, Y / 2); // Origin also acts as center of mass. getOrigin() for access.

	/*sf::Vector2f worldOrigin = */

	originOffset = { X / 2, Y / 2 };
	shape.setPosition(posX, posY);


	
	sf::Vector2f point;
	for (size_t i = 0; i < 4; i++) // Push our points into a vector
	{ 
		 //points.push_back(shape.getTransform().transformPoint(shape.getPoint(i))); // World coordinates
		 
		points.push_back(sf::Vector2f(posX + (shape.getPoint(i).x - size.x / 2)*cos(angle) - (shape.getPoint(i).y - size.y / 2)*sin(angle),
					  	posY + (shape.getPoint(i).y - size.y / 2)*cos(angle) + (shape.getPoint(i).x - size.x / 2)*sin(angle)));
		
		point = shape.getPoint(i);
	}

	for (size_t i = 0; i < points.size(); i++)
	{
		int j = (i + 1) % points.size();
		sf::Vector2f edge = points[j] - points[i]; // !
		// normalize(edge);
		axes.push_back(edge);
	}
	debugMode = false;
}


void Object::render(sf::RenderWindow& win)
{

	shape.setPosition(shape.getPosition().x, -shape.getPosition().y + win.getSize().y);
	win.draw(shape);
	shape.setPosition(shape.getPosition().x, -shape.getPosition().y - win.getSize().y);
}

void Object::updateAxes()
{
	// Hard code is best code

	for (size_t i = 0; i < 4; i++) // Push our points into a vector
	{
		//points[i] = shape.getTransform().transformPoint(shape.getPoint(i));
		points[i] = sf::Vector2f(posX + (shape.getPoint(i).x-size.x/2)*cos(angle) - (shape.getPoint(i).y-size.y/2)*sin(angle), 
			                     posY + (shape.getPoint(i).y-size.y/2)*cos(angle) + (shape.getPoint(i).x-size.x/2)*sin(angle));
	}

	for (size_t i = 0; i < points.size(); i++)
	{
		int j = (i + 1) % points.size();
		sf::Vector2f edge = points[j] - points[i];
		// normalize(edge);
		axes[i] = edge;
	}
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
		// We don't care how the object gained its velocity so mass isn't taken into account. Maybe in the future.
		posY = posY + vy  * deltaTime;
		vy = vy + g * deltaTime;
		posX = posX + vx * deltaTime;
	}

	if (rotatable) // Object can rotate.
	{
		angle = angle + angVel * deltaTime;
	}

	if (debugMode)
	{
		posX = sf::Mouse::getPosition(*window).x;
		posY = -sf::Mouse::getPosition(*window).y + window->getSize().y;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			angVel -= 0.5f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			angVel += 0.5f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			angle = 45.0f;
			setAngularVelocity(0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			angle = 0.0f;
			setAngularVelocity(0.0f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		{
			angle = 30.0f;
			setAngularVelocity(0.0f);
		}
	}

	shape.setRotation(RADTODEG * -angle);
	shape.setPosition(posX, posY);

	updateAxes(); // Get all axes for this object for SAT.

	// Might get huge
}

