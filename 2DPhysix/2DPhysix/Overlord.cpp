#include "Overlord.h"
#include <iostream>

void Overlord::run()
{
	window.setFramerateLimit(60);
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

		// Deltatime for Euler (Physics calculations, need to manually adjust this)
		dt = 0.01f;

		// Update and draw everything.
		update(dt);
		render(window);

		// Display drawn objects.
		window.display();

	}
}

void Overlord::init()
{
	// Create all the objects here...
	Object* box1 = createObject(100.0f, 100.0f, window.getSize().x / 2, 0.0f, 2.0f);
	box1->getShape().setFillColor(sf::Color::Red);
	box1->setHorizontalVelocity(-1.0f);
	box1->setVerticalVelocity(-5.0f);
	box1->setAngle(45.0f);

	Object* box2 = createObject(100.0f, 100.0f, window.getSize().x / 2, 100.0f, 2.0f);
	box2->getShape().setFillColor(sf::Color::Blue);
	box2->setHorizontalVelocity(1.0f);
	box2->setVerticalVelocity(-5.0f);
	box2->setAngle(45.0f);
	box2->setAngularVelocity(1.0f);
	
	// Ground / wall mass should be infinite, need to check visually for a proper value.
	Object* ground = createObject(window.getSize().x, 50.0f, window.getSize().x / 2, window.getSize().y - 50.0f, 0.0f, false, false);
	ground->getShape().setFillColor(sf::Color::Magenta);
	
}