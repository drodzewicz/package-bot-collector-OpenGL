
#ifndef CCOLIDER_HPP
#define CCOLIDER_HPP

#include "../libs.hpp"
#include "CMesh.hpp"

class CCollider
{

public:

	virtual bool isCollision(CCollider *_other) {
		return false;
	}

};

class CAABBCollider : public CCollider
{

public:
    CAABBCollider () { }
	// wskaznik do polozenia obiektu
	glm::vec3 * Position;
	
	// Dlugosc kazdego boku boxa
	glm::vec3 Size;

	CAABBCollider (glm::vec3 * _pos, glm::vec3 _size) {
		this->Position = _pos;
		this->Size = _size;
	}

	virtual bool isCollision(CCollider *_other)
	{
		CAABBCollider *other = dynamic_cast<CAABBCollider *>(_other);

		glm::vec3 minA = this->get_min();
		glm::vec3 maxA = this->get_max();

		glm::vec3 minB = other->get_min();
		glm::vec3 maxB = other->get_max();

		return 	(minA.x <= maxB.x && maxA.x >= minB.x) &&
        		(minA.y <= maxB.y && maxA.y >= minB.y) &&
        		(minA.z <= maxB.z && maxA.z >= minB.z);
	}

	inline glm::vec3 get_min() { return * this->Position - glm::vec3(this->Size.x/2, this->Size.y/2, this->Size.y/2); }
	inline glm::vec3 get_max() { return * this->Position + glm::vec3(this->Size.x/2, this->Size.y/2, this->Size.y/2); }

	bool isPointInsideAABB(glm::vec3 _point)
	{
		glm::vec3 min = this->get_min();
		glm::vec3 max = this->get_max();

  		return 	(_point.x >= min.x && _point.x <= max.x) &&
    	     	(_point.y >= min.y && _point.y <= max.y) &&
         		(_point.z >= min.z && _point.z <= max.z);
	}

};



#endif