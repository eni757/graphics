#include <draw2d/draw.hpp>

#include <algorithm>

#include <cmath>
#include <cassert>

#include <draw2d/sampler.hpp>
#include <draw2d/surface.hpp>

namespace
{
	inline
	unsigned classify_( Rect2F const& aRect, Vec2f const& aPoint )
	{
		unsigned ret{};
		if( aPoint.x < aRect.xmin )
			ret |= 1;
		else if( aPoint.x > aRect.xmin + aRect.width - 1.f )
			ret |= 2;
		
		if( aPoint.y < aRect.ymin )
			ret |= 4;
		else if( aPoint.y > aRect.ymin + aRect.height - 1.f )
			ret |= 8;
		
		return ret;
	}
}

// Implement in Task 1.3
void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	// TODO: your implementation goes here

	//draw triangle
	//find lowest x coordinate, go in lines from one side to other. check if pixel is in the triangle.
	//check pixel is in triangle using barry coords. 
	
	//cross product - find area of big triangle
	//find alpha, beta, gamma (sub traingles) a, b,y 
	//check that a, b, y properties hold so between 0 and 1. and a+b+y=1 are true. 
	//if true, is in the triangle, move onto pixels. 

	//based on how far pixel is from each of the points, set it to a colour. 
	//rinse and repeat for all pixel

}

// Implement in Task 2.3
void draw_triangle_tex_masked( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, Sampler2D const& aSampler, Vec2f aT0, Vec2f aT1, Vec2f aT2 )
{
	// TODO: your implementation goes here
}


// See Lecture 8
bool clip_line( Rect2F const& aTargetArea, Vec2f& aBegin, Vec2f& aEnd )
{
	// Note: If the line is clipped, update aBegin and aEnd correspondingly.
	// They are non-const references (&), meaning we can modify the values
	// passed in through them.
	//
	// Return true to indicate that the line should be drawn and false that
	// it should be discarded.

	// See https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
	auto b = classify_( aTargetArea, aBegin );
	auto e = classify_( aTargetArea, aEnd );

	while( true )
	{
		if( 0 == b && 0 == e )
			return true;
		
		if( b & e )
			return false;

		auto const next = e > b ? e : b;
		assert( next );

		float const dx = aEnd.x - aBegin.x;
		float const dy = aEnd.y - aBegin.y;

		float x, y;
		if( next & 8 )
		{
			x = aBegin.x + dx * (aTargetArea.ymin + aTargetArea.height -1.f - aBegin.y) / dy;
			y = aTargetArea.ymin + aTargetArea.height -1.f;
		}
		else if( next & 4 )
		{
			x = aBegin.x + dx * (aTargetArea.ymin - aBegin.y) / dy;
			y = aTargetArea.ymin;
		}
		else if( next & 2 )
		{
			x = aTargetArea.xmin + aTargetArea.width -1.f;
			y = aBegin.y + dy * (aTargetArea.xmin + aTargetArea.width -1.f - aBegin.x) / dx;
		}
		else // next & 1
		{
			x = aTargetArea.xmin;
			y = aBegin.y + dy * (aTargetArea.xmin - aBegin.x) / dx;
		}

		if( next == b )
		{
			aBegin.x = x;
			aBegin.y = y;
			b = classify_( aTargetArea, aBegin );
		}
		else
		{
			aEnd.x = x;
			aEnd.y = y;
			e = classify_( aTargetArea, aEnd );
		}
	}

	return true;
}

void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	draw_line_solid( aSurface, aSurface.clip_area(), aBegin, aEnd, aColor );
}
void draw_line_solid( Surface& aSurface, Rect2F const& aClipArea, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	if( !clip_line( aClipArea, aBegin, aEnd ) )
		return;

	Vec2f dd = aEnd - aBegin;

	if( std::abs(dd.x) > std::abs(dd.y) )
	{
		float const ystep = dd.y / dd.x;

		if( aBegin.x > aEnd.x ) 
			std::swap( aBegin, aEnd );

		float const end = aEnd.x + 0.5f;
		for( float x = aBegin.x+0.5f, y = aBegin.y+0.5f; x <= end; x += 1.f, y += ystep )
			aSurface.set_pixel_srgb( std::uint32_t(x), std::uint32_t(y), aColor );
	}
	else
	{
		float const xstep = dd.x / dd.y;

		if( aBegin.y > aEnd.y ) 
			std::swap( aBegin, aEnd );

		float const end = aEnd.y + 0.5f;
		for( float x = aBegin.x+0.5f, y = aBegin.y+0.5f; y <= end; y += 1.f, x += xstep )
			aSurface.set_pixel_srgb( std::uint32_t(x), std::uint32_t(y), aColor );
	}
}



void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	// TODO: (Exercise G.1) your implementation goes here. Can ignore for CW2.
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	// TODO: (Exercise G.1) your implementation goes here. Can ignore for CW2.
}


