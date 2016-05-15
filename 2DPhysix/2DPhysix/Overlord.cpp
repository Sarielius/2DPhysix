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
	


	Object* box1 = createObject(100.0f, 100.0f, 500, 100.0f, 2.0f, false);
	box1->getShape().setFillColor(sf::Color::Red);
	box1->setHorizontalVelocity(1.0f);
	box1->setVerticalVelocity(-2.0f);
	box1->setAngle(30.0f);

	Object* debugBox = createObject(100.0f, 100.0f, sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 0.0f, false);
	debugBox->getShape().setFillColor(sf::Color::White);
	debugBox->setDebugMode(true, &window);

	Object* box2 = createObject(100.0f, 100.0f, 780, 100.0f, 2.0f, false);
	box2->getShape().setFillColor(sf::Color::Blue);
	box2->setHorizontalVelocity(-1.0f);
	box2->setVerticalVelocity(-2.0f);
	box2->setAngle(45.0f);
	box2->setAngularVelocity(0.5f);

	Object* box3 = createObject(100.0f, 100.0f, 650.0f, 250.0f, 2.0f, false);
	box3->getShape().setFillColor(sf::Color::Cyan);
	box3->setHorizontalVelocity(1.0f);
	box3->setVerticalVelocity(-2.0f);
	box3->setAngle(45.0f);
	
	// Ground / wall mass should be infinite, need to check visually for a proper value, likely 0
	/*Object* ground = createObject(window.getSize().x, 50.0f, window.getSize().x / 2, window.getSize().y - 50.0f, 0.0f, false, false);
	ground->getShape().setFillColor(sf::Color::Magenta);*/
	
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
	std::vector<int> colAxisA;
	std::vector<int> colAxisB;
	//std::vector<sf::Vector2f> axis1 = obj1->getAxes();
	//std::vector<sf::Vector2f> axis2 = obj2->getAxes();

	for (size_t i = 0; i < obj1->axes.size(); i++)
	{
		Projection proj1 = getProjection(obj1->axes[i], obj1);
		Projection proj2 = getProjection(obj1->axes[i], obj2);

		double currentOverlap = getOverlap(proj1, proj2);

		if (currentOverlap == 0.0f) // "Touching" or not colliding at all
		{
			return;
		}

		if (currentOverlap < mtv.overlap || mtv.overlap == 0.0f)
		{
			mtv.overlap = currentOverlap;
			mtv.axis = obj1->axes[i];
			colAxisA.push_back(i);
		}

	}

	for (size_t i = 0; i < obj2->axes.size(); i++)
	{
		Projection proj1 = getProjection(obj2->axes[i], obj1);
		Projection proj2 = getProjection(obj2->axes[i], obj2);

		double currentOverlap = getOverlap(proj1, proj2);

		if (currentOverlap == 0.0f) // "Touching" or not colliding at all
		{
			return;
		}

		if (currentOverlap < mtv.overlap)
		{
			mtv.overlap = currentOverlap;
			mtv.axis = obj2->axes[i];
			colAxisB.push_back(i);
		}

	}

	// If get here a collision has occurred
	debugCounter++;
	std::cout << "Collisions: " << debugCounter << "\n" ;

	//resolveCollisions(obj1, obj2, mtv);
}

