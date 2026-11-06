#ifndef SPACESHIP_HPP_2F462327_57BF_433C_8AAE_6EA931BC1DE5
#define SPACESHIP_HPP_2F462327_57BF_433C_8AAE_6EA931BC1DE5

#include <array>

#include <draw2d/forward.hpp>
#include <draw2d/color.hpp>
#include <draw2d/image.hpp>
#include <draw2d/shape.hpp>

#include <vmlib/stdtypes.hpp>

// Line-based space ship
struct SpaceShipLines
{
	static constexpr float kRadius = 52.f;
	static constexpr std::array<Vec2f,2> kNozzles{
		Vec2f{ -30.f, -8.f },
		Vec2f{ -30.f, +8.f } 
	};

	LineStrip lines;
	ColorF color;

	void draw( Surface&, float aAngleRadians, Vec2f aTranslation ) const;
};

SpaceShipLines make_spaceship_lines();


// Task 2.1: Image atlas space ship
struct SpaceShipAtlas
{
	static constexpr float kRadius = 123.f;
	static constexpr char const* kSourceImage = "assets/goddard8x8.png";
	static constexpr std::array<Vec2f,4> kNozzles = {
		Vec2f{ -62.f, -78.f }, 
		Vec2f{ -54.f, -42.f },
		Vec2f{ -54.f, +42.f },
		Vec2f{ -62.f, +78.f }
	};

	std::unique_ptr<ImageRGBA> atlas;
	int atlasSize = 8*8, atlasWidth = 8;

	void draw( Surface&, float aAngleRadians, Vec2f aTranslation ) const;
};

SpaceShipAtlas make_spaceship_atlas();

// Task 2.2: Sampler-based space ship
struct SpaceShipSampler
{
	static constexpr float kRadius = 184.f;
	static constexpr char const* kSourceImage = "assets/phalanx.png";
	static constexpr std::array<Vec2f,2> kNozzles{
		Vec2f{ -62.f, -12.f },
		Vec2f{ -62.f, +12.f } 
	};

	std::unique_ptr<ImageRGBA> texture;

	void draw( Surface&, float aAngleRadians, Vec2f aTranslation ) const;
};

SpaceShipSampler make_spaceship_sampler();

#endif // SPACESHIP_HPP_2F462327_57BF_433C_8AAE_6EA931BC1DE5
