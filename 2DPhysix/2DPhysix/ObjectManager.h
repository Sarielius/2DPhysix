#pragma once
#include <vector>
#include "Object.h"

class ObjectManager
{
public:
	ObjectManager() {};
	~ObjectManager() {};

	void updateObjects()
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			objects[i]->update();
		}
	};

private:
	std::vector<Object*> objects;
};
