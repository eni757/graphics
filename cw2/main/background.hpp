#ifndef BACKGROUND_HPP_CBF3FF09_FAFE_4445_A3A0_FF95A6527EF5
#define BACKGROUND_HPP_CBF3FF09_FAFE_4445_A3A0_FF95A6527EF5

#include <memory>

#include <draw2d/forward.hpp>
#include <draw2d/color.hpp>

#include <vmlib/stdtypes.hpp>

#include "defaults.hpp"
#include "particle_field.hpp"

class Background final
{
	public:
		Background( RNG&, std::uint32_t aImageWidth, std::uint32_t aImageHeight );
		~Background();

	public:
		void update( Vec2f aPosition, Vec2f aMovementDelta );

		void draw( Surface& );

		void resize( std::uint32_t aImageWidth, std::uint32_t aImageHeight );

 
	public: // Configuration values. Mostly empirically determined
		static constexpr std::size_t kFarLayers = 3;

		static constexpr float kFarDensities[kFarLayers] = { 5e-5f, 8e-5f, 6e-5f };
		static constexpr float kFarSpeedMults[kFarLayers] = { 0.2f, 0.5f, 0.7f };
		static constexpr ColorF kFarColors[kFarLayers] = {
			{ .6f, .5f, .5f },
			{ .8f, .8f, .7f },
			{ .8f, .85f, .75f }
		};

		static constexpr float kNearDensity = 0.00013f;
		static constexpr float kNearSpeedMult = 1.f;
		static constexpr ColorF kNearColor = { .35f, .35f, .35f };

		static constexpr char const* kEarthPath = "assets/earth.png";
		static constexpr Vec2f kEarthCoord = { 1800.f, 500.f };

	private:
		ParticleField mFarField[kFarLayers];
		ParticleField mNearField;
		
		std::unique_ptr<ImageRGBA> mEarthSprite;

		Vec2f mCurrentPosition;
};

#endif // BACKGROUND_HPP_CBF3FF09_FAFE_4445_A3A0_FF95A6527EF5
