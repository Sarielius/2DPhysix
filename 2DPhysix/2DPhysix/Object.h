#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

class Object
{
public:
	Object(const float X, const float Y, const float positionX, const float positionY, const float mass, bool mov, bool rot, const float gravity);

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

	void normalize(sf::Vector2f& vec)
	{
		float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y);

		// Can't divide by zero.
		if (magnitude > 0)
		{
			vec = sf::Vector2f(vec.x / magnitude, vec.y / magnitude);
		}
	}

	std::vector<sf::Vector2f>& getAxes()
	{
		return axes;
	}

	void updateAxes();

	void update(float deltaTime);
	void render(sf::RenderWindow& win);

	std::vector<sf::Vector2f> points; // Contains shape points in order
	std::vector<sf::Vector2f> axes; // Contains edge normals for this object.

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

	
};

