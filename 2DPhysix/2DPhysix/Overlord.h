#pragma once
#include "Object.h"
#include <vector>

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

struct MTV
{
	sf::Vector2f axis = sf::Vector2f(0.0f, 0.0f);
	float overlap = 0.0f;
	int owner = 0;
};

struct Projection
{
	float min;
	float max;
};

struct Line
{
	sf::Vector2f point1;
	sf::Vector2f point2;

	bool operator==(const Line& other)
	{
		return this->point1 == other.point1 && this->point2 == other.point2;
	}
	
};

struct IntersectionData
{
	Line line;
	sf::Vector2f point;
};


class Overlord
{
public:
	Overlord(int w, int h) : window(sf::VideoMode(w, h), "2DPhysix Premium 2016"), 
		dt(0.0f), 
		debugCounter(0),
		e(1.0f)
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

	

	///////////////////// Mathstuff + physics /////////////////////



	// Returns dot product of two vectors, points, axis etc.
	/*float result = (vec1.x * vec2.x + vec1.y * vec2.y);
	return result;*/
	inline float dot(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
	{
		return (vec1.x * vec2.x + vec1.y * vec2.y);
	}

	// Perpendicular dot product
	inline float perpDot(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
	{
		return (vec1.y * vec2.x - vec1.x * vec2.y);
	}

	inline sf::Vector2f perp(const sf::Vector2f& vec)
	{
		sf::Vector2f perpendicularVector = { vec.y, -vec.x };
		return perpendicularVector;
	}

	// Gets the projection of a shape on a specific axis.
	Projection getProjection(const sf::Vector2f& axis, Object* obj)
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

	
	bool doIntersect(const sf::Vector2f& A1, const sf::Vector2f& A2, const sf::Vector2f& B1, const sf::Vector2f& B2)
	{
		sf::Vector2f lineA(A2.x - A1.x, A2.y - A1.y);
		sf::Vector2f lineB(B2.x - B1.x, B2.y - B1.y);
		
		// f is 0 if the lines are parallel
		double f = perpDot(lineA, lineB);
		if (!f)
		{
			return false; // Return false if parallel
		}

		sf::Vector2f lineC(B2.x - A2.x, B2.y - A2.y);
		double aa = perpDot(lineA, lineC);
		double bb = perpDot(lineB, lineC);

		if (f < 0)
		{
			if (aa > 0) return false;
			if (bb > 0) return false;
			if (aa < f) return false;
			if (bb < f) return false;
		}
		else
		{
			if (aa < 0) return false;
			if (bb < 0) return false;
			if (aa > f) return false;
			if (bb > f) return false;
		}

		return true; // This should work, hopefully
	}

	

	void simulate();
	void checkCollisions(Object* obj1, Object* obj2);
	// void resolveCollisions(Object* obj1, Object* obj2, const MTV& mtv); // tjsp
	IntersectionData getCollidingPoint(Object* obj1, Object* obj2);


	///////////////////// Public variables /////////////////////

	
	sf::RenderWindow window;

private:
	int debugCounter;
	std::vector<Object*> objects;

	// e = restitution, global for simplicity
	float e;

	float dt;
};

