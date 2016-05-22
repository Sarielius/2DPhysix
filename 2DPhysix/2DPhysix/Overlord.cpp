#include "Overlord.h"
#include <iostream>

void Overlord::run()
{
	window.setFramerateLimit(30);
	init();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) // Loop closes with the window
			{
				window.close();
			}
		}

		// Clear color
		window.clear(sf::Color::Black);

		// Deltatime for Euler (Physics calculations, need to manually adjust this, or take from sf::Clock.reset())
		dt = 0.01f;

		// Check collisions and resolve those that have occured.

		simulate();

		// Update and draw everything.
		update(dt);
		render(window);

		// Print stuff to console.
		debugPrint();
		
		// Display drawn objects.
		window.display();

	}
}

void Overlord::init()
{
	 //Create all the objects here...
	
	Object* debugBox = createObject(100.0f, 100.0f, sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 0.0f, false);
	debugBox->getShape().setFillColor(sf::Color::White);
	debugBox->setDebugMode(true, &window);

	Object* box1 = createObject(100.0f, 100.0f, 400, 200.0f, 2.0f, false);
	box1->getShape().setFillColor(sf::Color::Red);
	box1->setHorizontalVelocity(1.0f);
	box1->setVerticalVelocity(-2.0f);
	box1->setAngle(30.0f);

	Object* box2 = createObject(100.0f, 100.0f, 880, 200.0f, 2.0f, false);
	box2->getShape().setFillColor(sf::Color::Blue);
	box2->setHorizontalVelocity(-1.0f);
	box2->setVerticalVelocity(-2.0f);
	box2->setAngle(45.0f);
	box2->setAngularVelocity(0.5f);

	Object* box3 = createObject(100.0f, 100.0f, 200.0f, 500.0f, 2.0f, false);
	box3->getShape().setFillColor(sf::Color::Cyan);
	box3->setHorizontalVelocity(1.0f);
	box3->setVerticalVelocity(-2.0f);
	box3->setAngle(45.0f);

	Object* box4 = createObject(100.0f, 100.0f, 1080.0f, 500.0f, 2.0f, false);
	box4->getShape().setFillColor(sf::Color::Yellow);
	box4->setHorizontalVelocity(1.0f);
	box4->setVerticalVelocity(-2.0f);
	box4->setAngle(0.0f);
	
	Object* box5 = createObject(500.0f, 100.0f, 640.0f, 550.0f, 2.0f, false);
	box5->getShape().setFillColor(sf::Color::Green);
	box5->setAngle(0.0f);

	


    // Ground / wall mass should be infinite, need to check visually for a proper value, likely 0
	Object* ground = createObject(window.getSize().x, 50.0f, window.getSize().x / 2, window.getSize().y - 25.0f, 0.0f, false, false);
	ground->getShape().setFillColor(sf::Color::Magenta);

	
}


void Overlord::debugPrint()
{
	// All console printing here.

	/*Object* obj = objects[0];

	for (size_t i = 0; i < 4; i++) 
	{
		std::cout << "Object point " << i << ":" << "\nx: " << obj->points[i].x
			<< "\ny: " << obj->points[i].y << "\n";
	}*/


}

void Overlord::simulate() // Simulate using dirty hax or smth fam
{

	for (size_t i = 0; i < objects.size() - 1; i++)
	{
		for (size_t j = i + 1; j < objects.size(); j++)
		{
			checkCollisions(objects[i], objects[j]);
		}
	}
}

void Overlord::checkCollisions(Object* obj1, Object* obj2)
{
	MTV mtv;

	for (size_t i = 0; i < obj1->axes.size(); i++)
	{
		Projection proj1 = getProjection(obj1->axes[i], obj1);
		Projection proj2 = getProjection(obj1->axes[i], obj2);

		float overlapA = getOverlap(proj1, proj2);

		if (overlapA == 0.0f) // "Touching" or not colliding at all
		{
			return;
		}

		if (overlapA < mtv.overlap || mtv.overlap == 0.0f)
		{
			mtv.overlap = overlapA;
			mtv.axis = obj1->axes[i];
			mtv.owner = 1;
		}
	
	}

	for (size_t i = 0; i < obj2->axes.size(); i++)
	{
		Projection proj1 = getProjection(obj2->axes[i], obj1);
		Projection proj2 = getProjection(obj2->axes[i], obj2);

		float overlapB = getOverlap(proj1, proj2);

		if (overlapB == 0.0f) // "Touching" or not colliding at all
		{
			return;
		}

		if (overlapB < mtv.overlap)
		{
			mtv.overlap = overlapB;
			mtv.axis = obj2->axes[i];
			mtv.owner = 2;
		}
	}

	sf::Vector2f collidingPoint = getCollidingPoint(obj1, obj2);
	sf::Vector2f relativeCollidingPoint;

	sf::Vector2f originA = obj1->getShape().getTransform().transformPoint(obj1->getShape().getOrigin());
	sf::Vector2f originB = obj2->getShape().getTransform().transformPoint(obj2->getShape().getOrigin());

	sf::Vector2f vectorToCollisionA;
	sf::Vector2f vectorToCollisionB;

	sf::Vector2f collisionNormal;

	// If get here a collision has occurred
	debugCounter++;
	//std::cout << "Collisions: " << debugCounter << "\n" ;
	// std::cout << "\nColliding point:" << "\nX " << collidingPoint.x << "\nY: " << collidingPoint.y << "\n";
	
	if (obj1->ownsPoint(collidingPoint))
	{
		// Relative colliding point is the point in the OTHER object's local coordinates
		relativeCollidingPoint = obj2->getShape().getInverseTransform().transformPoint(collidingPoint);
		
		// The vector from the origin to the collision point 
		vectorToCollisionA = collidingPoint - originA;

		// Origin offset is basically the objects origin in local coordinates.
		vectorToCollisionB = relativeCollidingPoint - obj2->getOriginOffset();
	}
		
	if (obj2->ownsPoint(collidingPoint)) // Other object must own the point if the first one does not.
	{
		relativeCollidingPoint = obj1->getShape().getInverseTransform().transformPoint(collidingPoint);
		
		vectorToCollisionB = collidingPoint - originB;

		vectorToCollisionA = relativeCollidingPoint - obj1->getOriginOffset(); 
	}


	std::cout << "\nRel. Col. point:" << "\nX " << relativeCollidingPoint.x << "\nY: " << relativeCollidingPoint.y << "\n";
	std::cout << "\nVec to pointA:" << "\nX " << vectorToCollisionA.x << "\nY: " << vectorToCollisionA.y << "\n";
	std::cout << "\nVec to pointB:" << "\nX " << vectorToCollisionB.x << "\nY: " << vectorToCollisionB.y << "\n";
	std::cout << "\nMTV owner = " << mtv.owner << "\n";

	//resolveCollisions(obj1, obj2, mtv); Or just do this here? Is there a point in chaining this onwards?
}

sf::Vector2f& Overlord::getCollidingPoint(Object* obj1, Object* obj2)
{
	std::vector<sf::Vector2f> points1 = obj1->getPoints();
	std::vector<sf::Vector2f> points2 = obj2->getPoints();
	std::vector<Line> lines;

	for (size_t i = 0; i < points1.size(); i++)
	{
		int k = (i + 1) % points1.size();

		for (size_t j = 0; j < points2.size(); j++)
		{
			int g = (j + 1) % points2.size();

			if (doIntersect(points1[i], points1[k], points2[j], points2[g]))
			{
				bool line1Duplicate = false;
				bool line2Duplicate = false;

				Line line1 = { points1[i], points1[k] };
				Line line2 = { points2[j], points2[g] };

				if (!lines.empty())
				{
					for (size_t i = 0; i < lines.size(); i++)
					{
						if (lines[i] == line1)
						{
							line1Duplicate = true;
						}
					}

					for (size_t i = 0; i < lines.size(); i++)
					{
						if (lines[i] == line2)
						{
							line2Duplicate = true;
						}
					}

					if (!line1Duplicate)
					{
						lines.push_back(line1);
					}

					if (!line2Duplicate)
					{
						lines.push_back(line2);
					}
					
				}
				else // First two are ALWAYS unique.
				{
					lines.push_back(line1);
					lines.push_back(line2);
				}
			}
		}
	}

	// Can use 0.0f , 0.0f as no point found.
	// If the "penetrating point happens by some miracle to be in world origin,
	// we ignore the collision until it has moved a tiny bit further.
	sf::Vector2f point(0.0f, 0.0f); 
	
	for (size_t i = 0; i < lines.size() - 1; i++)
	{
		for (size_t j = i + 1; j < lines.size(); j++)
		{
			if (lines[i].point1 == lines[j].point1 || lines[i].point1 == lines[j].point2)
			{
				point = lines[i].point1;
			}
			if (lines[i].point2 == lines[j].point1 || lines[i].point2 == lines[j].point2)
			{
				point = lines[i].point2;
			}
		}	
	}

	return point;
}

void Overlord::resolveCollisions(Object* obj1, Object* obj2, const MTV& mtv)
{

}