#ifndef PARTICLE_FIELD_HPP_A738A985_6705_4519_9EA6_1AE3498F39E0
#define PARTICLE_FIELD_HPP_A738A985_6705_4519_9EA6_1AE3498F39E0

#include <vector>

#include <cstdlib>

#include <draw2d/forward.hpp>
#include <draw2d/color.hpp>
#include <vmlib/stdtypes.hpp>

#include "defaults.hpp"

class ParticleField final
{
	public:
		ParticleField( 
			RNG& aRNG,
			std::uint32_t aImageWidth, std::uint32_t aImageHeight,
			ColorF const& aParticleColor,
			float aParticleDensity,
			float aParticleSpeedMult = 1.f,
			float aPadding = 50.f
		);

	public:
		void update( Vec2f aMovementDelta ) noexcept;

		void draw( Surface& ) const;

		void resize( std::uint32_t aImageWidth, std::uint32_t aImageHeight );
	
	private:
		std::vector<Vec2f> mParticles;

		ColorU8_sRGB mColor;

		float mParticleSpeedMult;
		float mParticleDensity;
		float mPadding;

		Vec2f mVisibleExtent;
		Vec2f mBoxMin, mBoxMax;

		RNG& mRNG;
};

#endif // PARTICLE_FIELD_HPP_A738A985_6705_4519_9EA6_1AE3498F39E0
