#ifndef ASTEROID_FIELD_HPP_4BF0D527_FC96_4FB5_A3D9_5CD4CE9BC3B3
#define ASTEROID_FIELD_HPP_4BF0D527_FC96_4FB5_A3D9_5CD4CE9BC3B3

#include <span>
#include <vector>

#include <cstdlib>

#include <draw2d/forward.hpp>
#include <draw2d/color.hpp>

#include <vmlib/stdtypes.hpp>

#include "defaults.hpp"

/** Asteroid field
 *
 * Similar to the particle fields, the asteroid field only exists around the
 * "player" (i.e., it is always centered on the screen's position). This gives
 * the appearance of an infinite asteroid field (without us having to track an
 * ever increasing amount of asteroids). Like the particle field, the asteroid
 * field extends a bit outside of the field of view. This means that asteroids
 * do not disappear the moment they are outside of the visible region (i.e., if
 * an asteroid exits the screen, the player turns around immediately, the same
 * asteroid still exists).
 *
 * You can enumerate all active asteroids (visible and invisible) with the
 * asteroids() method.
 */
class AsteroidField
{

	public:
		struct Asteroid
		{
			Vec2f pos;
			Vec2f vel;

			Mat22f rot;
			float radpersec;

			Vec2f aabbMin, aabbMax;

			bool exploding;
		};

	public:
		AsteroidField(
			RNG&,
			std::uint32_t aImageWidth, std::uint32_t aImageHeight,
			float aDensity = 1e-5f,
			float aInitialSpeedStddev = 100.f,
			float aMaximumSpeed = 500.f,
			float aInitialRotStddev = 1.5f,
			float aPadding = 300.f
		);

		~AsteroidField();

		// Not copyable nor movable
		AsteroidField( AsteroidField const& ) = delete;
		AsteroidField& operator= (AsteroidField const&) = delete;

	public:
		/* explode() marks the aAsteroidIndex:th asteroid to explode on the
		 * next update(). It is safe to call while iterating over the span
		 * returned by asteroids().
		 */
		void explode( std::size_t aAsteroidIndex );
		void update( float aElapsedTimeSec, Vec2f const& aMovement );

		void draw( Surface& ) const;

		void resize( std::uint32_t aWidth, std::uint32_t aHeight );

		std::span<Asteroid const> asteroids() const;
		std::span<TriangleFan const> shapes() const;

	private:
		struct Piece_
		{
			Vec2f pa, pb, pc;
			ColorF ca, cb, cc;

			Vec2f pos;
			Mat22f rot;

			Vec2f vel;
			float radpersec;

			float remainingLife;
		};
		struct Debris_
		{
			std::vector<Piece_> pieces;
		};

	private:
		void explode_( std::size_t );
		void compute_asteroid_aabbs_();

	private:
		Vec2f mBoundsMin, mBoundsMax;
		Vec2f mExactExtent, mActualExtent;
		
		std::vector<Asteroid> mAsteroids;
		std::vector<TriangleFan> mShapes;

		std::vector<Debris_> mDebris;

		float mInitialSpeed, mMaximumSpeed;
		float mInitialRot;
		float mPadding, mDensity;

		RNG& mRNG;
};

#endif // ASTEROID_FIELD_HPP_4BF0D527_FC96_4FB5_A3D9_5CD4CE9BC3B3
