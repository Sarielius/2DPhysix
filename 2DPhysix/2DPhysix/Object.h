#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class Object
{
public:
	Object(float X, float Y, float positionX, float positionY, float mass, bool mov, bool rot, float gravity);

	sf::RectangleShape& getShape()
	{
		return shape;
	}

	void setHorizontalVelocity(float vel)
	{
		vx = vel * forceMultiplier;
	}

	void setVerticalVelocity(float vel)
	{
		vy = vel * forceMultiplier;
	}

	void setAngle(float ang)
	{
		angle = ang;
	}

	void setAngularVelocity(float vel)
	{
		angVel = vel * forceMultiplier;
	}

	void setForceMultiplier(float value)
	{
		forceMultiplier = value;
	}

	/*std::vector<sf::Vector2f>& getPointVector()
	{
		return points;
	}*/

	std::vector<sf::Vector2f>& getAxisVector()
	{
		return axis;
	}

	void updateAxis();

	void update(float deltaTime);
	void render(sf::RenderWindow& win);

private:
	sf::RectangleShape shape;
	bool movable; // Can the object be moved by anything.
	bool rotatable; // Can the object spin.
	float g; // Gravity.
	float vx; // Horizontal speed.
	float vy; // Vertical speed.
	float posX; // Horizontal location.
	float posY; // Vertical location.
	float angle; // Rotation of the object in degrees.
	float angVel; // Angular velocity, how fast the object spins.
	float mass; // Mass of the object. Only used in collision handling.
	float forceMultiplier; // Multiplies velocities etc.

	std::vector<sf::Vector2f> points; // Contains vertexes in order.
	std::vector<sf::Vector2f> axis; // Contains
};

