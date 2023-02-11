#pragma once

enum class OBJ_GROUP { USER };

class OBJECT
{
private:
	OBJ_GROUP	group;
	float		x, y, z;

public:
	void move(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

