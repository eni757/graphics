#include "spaceship.hpp"

#include <numbers>

#include <cmath>
#include <cassert>

#include <draw2d/draw.hpp>
#include <draw2d/image.hpp>
#include <draw2d/sampler.hpp>
#include <draw2d/surface.hpp>

#include <vmlib/transform2d.hpp>

// Line-based space ship
void SpaceShipLines::draw( Surface& aSurface, float aAngleRadians, Vec2f aTranslation ) const
{
	auto rot = make_rotation_2d( aAngleRadians );
	lines.draw( aSurface, color, rot, aTranslation );
}

SpaceShipLines make_spaceship_lines()
{
	static constexpr float xs[] = { 250.f, 200.f, 150.f, 100.f, 000.f, 040.f, -50.f, -140.f, -170.f };
	static constexpr float ys[] = { 190.f, 180.f, 70.f, 50.f, 30.f, 20.f };

	LineStrip spaceship{ { 
		{ 0.2f * xs[0], 0.2f * +ys[5] }, // upper half. starts at front, goes towards the back
		{ 0.2f * xs[1], 0.2f * +ys[3] },
		{ 0.2f * xs[2], 0.2f * +ys[3] },
		{ 0.2f * xs[3], 0.2f * +ys[4] }, 
		{ 0.2f * xs[4], 0.2f * +ys[4] },
		{ 0.2f * xs[4], 0.2f * +ys[2] },
		{ 0.2f * xs[5], 0.2f * +ys[1] },
		{ 0.2f * xs[6], 0.2f * +ys[0] },
		{ 0.2f * xs[8], 0.2f * +ys[2] },
		{ 0.2f * xs[7], 0.2f * +ys[3] },

		{ 0.2f * xs[7], 0.2f * -ys[3] }, // lower half, starts at the back and goes towards the front
		{ 0.2f * xs[8], 0.2f * -ys[2] }, // this is essentially the same as the upper half, except in reverse.
		{ 0.2f * xs[6], 0.2f * -ys[0] },
		{ 0.2f * xs[5], 0.2f * -ys[1] },
		{ 0.2f * xs[4], 0.2f * -ys[2] },
		{ 0.2f * xs[4], 0.2f * -ys[4] },
		{ 0.2f * xs[3], 0.2f * -ys[4] }, 
		{ 0.2f * xs[2], 0.2f * -ys[3] },
		{ 0.2f * xs[1], 0.2f * -ys[3] },
		{ 0.2f * xs[0], 0.2f * -ys[5] }, 

		{ 0.2f * xs[0], 0.2f * +ys[5] } // link back to beginning (connects both sides at the "front")
	} };

	return SpaceShipLines{ std::move(spaceship), { 0.2f, 0.4f, 0.7f } };
}

// Image atlas space ship
void SpaceShipAtlas::draw( Surface& aSurface, float aAngleRadians, Vec2f aTranslation ) const
{
	// TODO: Your implementation here
}

SpaceShipAtlas make_spaceship_atlas()
{
	// TODO: Your implementation here
	return {};
}


// Sampler-based space ship
void SpaceShipSampler::draw( Surface& aSurface, float aAngleRadians, Vec2f aTranslation ) const
{
	// TODO: Your implementation here
}

SpaceShipSampler make_spaceship_sampler()
{
	// TODO: Your implementation here
	return {};
}

