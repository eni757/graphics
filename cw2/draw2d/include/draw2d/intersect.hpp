#ifndef INTERSECT_HPP_145FA289_6431_46A5_8213_BBC5234B5370
#define INTERSECT_HPP_145FA289_6431_46A5_8213_BBC5234B5370

// IMPORTANT: DO NOT CHANGE THIS FILE WHEN YOU ARE SOLVING COURSEWORK 2!
//
// For CW2, the intersect.hpp file must remain exactly as it is. In particular,
// you must not change any of the function prototypes in this header.

#include <vmlib/stdtypes.hpp>

// Task 2.4
bool intersect_circle_aabb(
	Vec2f aCircleCenter, float aRadius,
	Vec2f aAABBMin, Vec2f aAABBMax 
) noexcept;

bool intersect_circle_tri(
	Vec2f aCircleCenter, float aRadius,
	Vec2f aP0, Vec2f aP1, Vec2f aP2
) noexcept;

#include "intersect.inl"
#endif // INTERSECT_HPP_145FA289_6431_46A5_8213_BBC5234B5370
