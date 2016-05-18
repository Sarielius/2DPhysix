#pragma once
#include "Object.h"
#include <vector>

struct MTV
{
	sf::Vector2f axis = sf::Vector2f(0.0f, 0.0f);
	float overlap = 0.0f;
};

struct Projection
{
	float min;
	float max;
};

class Overlord
{
public:
	Overlord(int w, int h) : window(sf::VideoMode(w, h), "2DPhysix Premium 2016"), dt(0.0f), debugCounter(0)
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
	Object* createObject(float sizeX, float sizeY, float posX, float posY, float mass, bool movable = true, bool rotatable = true, float gravity = -10.0f)
	{
		// Gravity multiplier, need to test for a better value
		gravity = gravity * 100;
		Object* object = new Object(sizeX, sizeY, posX, posY, mass, movable, rotatable, gravity);
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

	void debugPrint();

	// Main loop
	void run();

	// Create objects and add them to the vector.
	void init();

	sf::RenderWindow window;

	// Mathstuff + physics

	// Returns dot product of two vectors, points, axis etc.
	float dot(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
	{
		// Could shorten this, longer for clarity.
		float result = (vec1.x * vec2.x + vec1.y * vec2.y);
		return result;
	}

	// Gets the projection of a shape on a specific axis.
	Projection& getProjection(const sf::Vector2f& axis, Object* obj)
	{
		Projection proj;

		proj.min = dot(axis, obj->points[0]);
		proj.max = proj.min;

		for (size_t i = 1; i < obj->points.size(); i++)
		{
			float val = dot(axis, obj->points[i]);
			if (val < proj.min)
			{
				proj.min = val;
			}
			if (val > proj.max)
			{
				proj.max = val;
			}
		}

		return proj;
	}

	float getOverlap(const Projection& proj1, const Projection& proj2)
	{
		if (proj1.max < proj2.min || proj2.max < proj1.min)
		{
			// Projections don't overlap.
			return 0.0f;
		}

		// Projections overlap if we get this far.

		float overlap1 = proj1.max - proj2.min;
		float overlap2 = proj2.max - proj1.min;

		if ( overlap1 < overlap2)
		{
			return overlap1;
		}
		else
		{
			return overlap2;
		}
	}

	void simulate();
	void checkCollisions(Object* obj1, Object* obj2);
	void resolveCollisions(Object* obj1, Object* obj2, const MTV& mtv); // tjsp
	sf::Vector2f& getCollidingPoint(Object* obj1, Object* obj2);


private:
	int debugCounter;
	std::vector<Object*> objects;

	float dt;
};

