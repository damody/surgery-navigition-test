#pragma once
#include "math\Vector2.h"


class AABB2D
{
public:
	void AddPoint(const Vector2& p);
	AABB2D(){}
	AABB2D(const Vector2& orgin)
		:mMin(orgin), mMax(orgin)
	{}
	void Larger(float val);
	void SetBounding(float left, float right, float top, float down);
	bool IsContain(const AABB2D& rhs);
	
	bool IsCollision(const AABB2D& rhs);
	Vector2 mMin, mMax;
};
