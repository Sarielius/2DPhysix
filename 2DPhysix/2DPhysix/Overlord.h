#pragma once
#include "Object.h"
#include <vector>

class Overlord
{
public:
	Overlord(int w, int h) : window(sf::VideoMode(w, h), "2DPhysix Premium 2016"), dt(0.0f)
	{};

	~Overlord()
	{
		for (auto &object : objects)
		{
			delete object;
		}
		objects.clear();
	}

	// Function which is used to create the objects.
	Object* createObject(float sizeX, float sizeY, float posX, float posY, bool movable = true, bool rotatable = true, float gravity = -10.0f)
	{
		// Gravity multiplier, need to test for a better value
		gravity = gravity * 100;
		Object* object = new Object(sizeX, sizeY, posX, posY, movable, rotatable, gravity);
		objects.push_back(object);
		return object;
	}

	// Update all the objects.
	void update(float deltaTime)
	{
		for (auto &object : objects)
		{
			object->update(deltaTime);
		}
	}

	// Draw everything
	void render(sf::RenderWindow& win)
	{
		for (auto &object : objects)
		{
			object->render(win);
		}
	}

	// Main loop
	void run();

	// Create objects add them to the vector.
	void init();

	sf::RenderWindow window;

private:
	
	std::vector<Object*> objects;
	float dt;
};

