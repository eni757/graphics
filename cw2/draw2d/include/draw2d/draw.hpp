#ifndef DRAW_HPP_E912C408_CE39_462A_BB84_1BCF3B4C5F3F
#define DRAW_HPP_E912C408_CE39_462A_BB84_1BCF3B4C5F3F

// IMPORTANT: DO NOT CHANGE THIS FILE WHEN YOU ARE SOLVING COURSEWORK 2!
//
// For CW2, the draw.hpp file must remain exactly as it is. In particular, you
// must not change any of the function prototypes in this header.

#include "forward.hpp"

#include "rect.hpp"
#include "color.hpp"

#include <vmlib/stdtypes.hpp>

// Implement in CW2:
// Task 1.3
void draw_triangle_interp(
	Surface&,
	Vec2f aP0, Vec2f aP1, Vec2f aP2,
	ColorF aC0, ColorF aC1, ColorF aC2
);

// Task 2.3
void draw_triangle_tex_masked(
	Surface&,
	Vec2f aP0, Vec2f aP1, Vec2f aP2,
	Sampler2D const&,
	Vec2f aT0, Vec2f aT1, Vec2f aT2
);

// Provided to you
// See Lecture 8
bool clip_line( Rect2F const&, Vec2f& aBegin, Vec2f& aEnd );

void draw_line_solid(
	Surface&,
	Vec2f aBegin, Vec2f aEnd,
	ColorU8_sRGB
);
void draw_line_solid(
	Surface&,
	Rect2F const&,
	Vec2f aBegin, Vec2f aEnd,
	ColorU8_sRGB
);

// From Exercise G.1
// You can ignore these in Coursework 2
void draw_rectangle_solid(
	Surface&,
	Vec2f aMinCorner, Vec2f aMaxCorner,
	ColorU8_sRGB
);

void draw_rectangle_outline(
	Surface&,
	Vec2f aMinCorner, Vec2f aMaxCorner,
	ColorU8_sRGB
);


#endif // DRAW_HPP_E912C408_CE39_462A_BB84_1BCF3B4C5F3F
