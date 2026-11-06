#ifndef EXHAUST_HPP_2C3F2F81_479A_4DC4_8591_AB3408B31F65
#define EXHAUST_HPP_2C3F2F81_479A_4DC4_8591_AB3408B31F65

#include <span>
#include <memory>
#include <vector>

#include <draw2d/forward.hpp>

#include <vmlib/stdtypes.hpp>

#include "defaults.hpp"

/* Exhaust trail
 *
 * Simple particle system. Particles are "thrown" out the back and essentially
 * stationary. We use relatively few (and small) particles, as naive alpha
 * blending implementations are quite expensive.
 */
class Exhaust
{
	public:
		static constexpr char const* kSpritePath = "assets/puff.png";

	public:
		explicit Exhaust( RNG&, std::span<Vec2f const> aEmitters, std::size_t aParticleCount = 256 );
		~Exhaust();
		
		// Not copyable nor movable
		Exhaust( Exhaust const& ) = delete;
		Exhaust& operator= (Exhaust const&) = delete;

	public:
		void update( 
			float aElapsedTimeSec, 
			Vec2f const& aMovement,
			Vec2f const& aPosition,
			float aAngleInRadians,
			bool aAccelerating 
		);

		void draw( Surface& ) const;

	private:
		struct Particle_
		{
			Vec2f pos;
			Vec2f vel;
			float life;
		};

	private:
		std::size_t mMaxCount;
		float mCooldown;
	
		std::vector<Particle_>  mParticles;
		std::unique_ptr<ImageRGBA> mSprite;

		std::vector<Vec2f> mEmitters;

		RNG& mRNG;
};

#endif // EXHAUST_HPP_2C3F2F81_479A_4DC4_8591_AB3408B31F65
