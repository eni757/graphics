#include "asteroid_field.hpp"

#include <limits>
#include <random>
#include <numbers>
#include <algorithm>

#include <cassert>

#include <draw2d/draw.hpp>
#include <draw2d/shape.hpp>

#include <vmlib/transform2d.hpp>

#include "asteroid.hpp"

AsteroidField::AsteroidField( RNG& aRNG, std::uint32_t aWidth, std::uint32_t aHeight, float aDensity, float aInitialSpeedStddev, float aMaximumSpeed, float aInitialRotStddev, float aPadding )
	: mInitialSpeed( aInitialSpeedStddev )
	, mMaximumSpeed( aMaximumSpeed )
	, mInitialRot( aInitialRotStddev )
	, mPadding( aPadding )
	, mDensity( aDensity )
	, mRNG( aRNG )
{
	// Compute area of simulation
	mExactExtent = Vec2f{ float(aWidth), float(aHeight) };

	mBoundsMin = Vec2f{ -mPadding, -mPadding };
	mBoundsMax = mExactExtent + Vec2f{ mPadding, mPadding };

	mActualExtent = mBoundsMax - mBoundsMin;

	// Generate initial asteroids
	float const numAsteroidsf = mActualExtent.x*mActualExtent.y * mDensity;
	std::size_t const numAsteroids = std::size_t(numAsteroidsf+0.5f);
	
	mAsteroids.resize( numAsteroids );
	mShapes.reserve( numAsteroids ); // reserve! not resize!

	using Uniform_ = std::uniform_real_distribution<float>;
	using Normal_ = std::normal_distribution<float>;

	Uniform_ xpos{ mBoundsMin.x, mBoundsMax.x };
	Uniform_ ypos{ mBoundsMin.y, mBoundsMax.y };

	Uniform_ angle( 0.f, 2*std::numbers::pi_v<float> );

	Normal_ vvel{ 0.f, mInitialSpeed };
	Normal_ rots{ 0.f, mInitialRot };

	for( std::size_t i = 0; i < numAsteroids; ++i )
	{
		auto& astr = mAsteroids[i];
		astr.pos = Vec2f{ xpos( mRNG ), ypos( mRNG ) };
		astr.vel = Vec2f{ vvel( mRNG ), vvel( mRNG ) };

		astr.rot = make_rotation_2d( angle( mRNG ) );
		astr.radpersec = rots( mRNG );

		// Don't break the speed limits. The space police will get you!
		astr.vel.x = std::clamp( astr.vel.x, -mMaximumSpeed, +mMaximumSpeed );
		astr.vel.y = std::clamp( astr.vel.y, -mMaximumSpeed, +mMaximumSpeed );

		astr.exploding = false;

		// Create shape
		mShapes.emplace_back( make_asteroid( mRNG ) );
	}

	compute_asteroid_aabbs_();
}

AsteroidField::~AsteroidField() = default;

void AsteroidField::explode( std::size_t aAsteroidIndex )
{
	assert( aAsteroidIndex < mAsteroids.size() );
	mAsteroids[aAsteroidIndex].exploding = true;
}

void AsteroidField::update( float aElapsed, Vec2f const& aTransl )
{
	// Explode asteroids
	std::size_t last = 0;
	for( std::size_t i = 0; i < mAsteroids.size(); ++i )
	{
		if( mAsteroids[i].exploding )
		{
			explode_( i );
			continue;
		}

		if( last != i )
		{
			mAsteroids[last] = std::move(mAsteroids[i]);
			mShapes[last] = std::move(mShapes[i]);
		}

		++last;
	}

	mAsteroids.erase( mAsteroids.begin()+last, mAsteroids.end() );
	mShapes.erase( mShapes.begin()+last, mShapes.end() );

	// Update asteroids
	auto const numAsteroids = mAsteroids.size();
	assert( numAsteroids == mShapes.size() );

	using Uniform_ = std::uniform_real_distribution<float>;
	using Normal_ = std::normal_distribution<float>;

	Uniform_ xpos{ mBoundsMin.x, mBoundsMax.x };
	Uniform_ ypos{ mBoundsMin.y, mBoundsMax.y };

	Uniform_ angle( 0.f, 2*std::numbers::pi_v<float> );

	Normal_ vvel{ 0.f, mInitialSpeed };
	Normal_ rots{ 0.f, mInitialRot };

	for( std::size_t i = 0; i < numAsteroids; ++i )
	{
		auto& astr = mAsteroids[i];
		astr.pos += astr.vel * aElapsed - aTransl;

		// If the asteroid is outside of the simulation area, replace it
		// with a fresh one.
		//
		// The method here isn't entirely optimal. The density of asteroids on
		// screen will reduce slightly over time (until some minimum) if the
		// player is standing still. New asteroids are generated with random
		// movement vectors. The random vectors are picked from a symmetric
		// normal distribution, meaning that the asteroid has a fair chance to
		// move off-screen without ever becoming visible.
		if( astr.pos.x < mBoundsMin.x || astr.pos.x > mBoundsMax.x || astr.pos.y < mBoundsMin.y || astr.pos.y > mBoundsMax.y )
		{
			if( astr.pos.x < mBoundsMin.x )
			{
				astr.pos.x = mBoundsMax.x - mPadding/2.f;
				astr.pos.y = ypos( mRNG );
			}
			else if( astr.pos.x > mBoundsMax.x )
			{
				astr.pos.x = mBoundsMin.x + mPadding/2.f;
				astr.pos.y = ypos( mRNG );
			}
			else if( astr.pos.y < mBoundsMin.y )
			{
				astr.pos.x = xpos( mRNG );
				astr.pos.y = mBoundsMax.y - mPadding/2.f;
			}
			else if( astr.pos.y > mBoundsMax.y )
			{
				astr.pos.x = xpos( mRNG );
				astr.pos.y = mBoundsMin.y + mPadding/2.f;
			}

			astr.vel = Vec2f{ vvel( mRNG ), vvel( mRNG ) };

			astr.rot = make_rotation_2d( angle( mRNG ) );
			astr.radpersec = rots( mRNG );

			// Don't break the speed limits. The space police will get you!
			astr.vel.x = std::clamp( astr.vel.x, -mMaximumSpeed, +mMaximumSpeed );
			astr.vel.y = std::clamp( astr.vel.y, -mMaximumSpeed, +mMaximumSpeed );

			astr.exploding = false;

			// Create shape
			mShapes[i] = make_asteroid( mRNG );
		}
		else
		{
			astr.rot = make_rotation_2d( astr.radpersec * aElapsed ) * astr.rot;
		}
	}

	// Update debris
	for( auto& debris : mDebris )
	{
		for( auto& piece : debris.pieces )
		{
			piece.pos += piece.vel * aElapsed - aTransl;
			piece.rot = make_rotation_2d( piece.radpersec * aElapsed ) * piece.rot;

			piece.remainingLife -= aElapsed;
		}

		// Compact dead pieces
		debris.pieces.erase( 
			std::remove_if( debris.pieces.begin(), debris.pieces.end(),
				[] (auto const& aX) { return aX.remainingLife <= 0.f; } 
			),
			debris.pieces.end()
		);
	}

	// Remove empty debris
	std::size_t const before = mDebris.size();
	mDebris.erase( 
		std::remove_if( mDebris.begin(), mDebris.end(),
			[] (auto const& aX) { return 0 == aX.pieces.size(); } 
		),
		mDebris.end()
	);
	std::size_t const after = mDebris.size();
	
	// Spawn replacement asteroids
	// We want these to spawn outside of the visible area. 
	std::uniform_int_distribution<unsigned> region(0,3);
	
	for( std::size_t n = before-after; n > 0; --n )
	{
		auto& astr = mAsteroids.emplace_back();

		switch( region(mRNG) )
		{
			case 0:
				astr.pos.x = mBoundsMax.x - mPadding/2.f;
				astr.pos.y = ypos( mRNG );
				break;
			case 1:
				astr.pos.x = mBoundsMin.x + mPadding/2.f;
				astr.pos.y = ypos( mRNG );
				break;
			case 2:
				astr.pos.x = xpos( mRNG );
				astr.pos.y = mBoundsMax.y - mPadding/2.f;
				break;
			case 3:
				astr.pos.x = xpos( mRNG );
				astr.pos.y = mBoundsMin.y + mPadding/2.f;
				break;
		}

		astr.vel = Vec2f{ vvel( mRNG ), vvel( mRNG ) };

		astr.rot = make_rotation_2d( angle( mRNG ) );
		astr.radpersec = rots( mRNG );

		astr.vel.x = std::clamp( astr.vel.x, -mMaximumSpeed, +mMaximumSpeed );
		astr.vel.y = std::clamp( astr.vel.y, -mMaximumSpeed, +mMaximumSpeed );

		astr.exploding = false;

		mShapes.emplace_back( make_asteroid( mRNG ) );
	}

	compute_asteroid_aabbs_();
}


void AsteroidField::draw( Surface& aSurface ) const
{
	// Draw asteroids
	auto const numAsteroids = mAsteroids.size();
	assert( numAsteroids == mShapes.size() );

	for( std::size_t i = 0; i < numAsteroids; ++i )
	{
		auto const& astr = mAsteroids[i];
		auto const& shape = mShapes[i];

		// Performance: culling asteroids here would remove some work; right
		// now each triangle will be culled individually.

		shape.draw(
			aSurface,
			astr.rot,
			astr.pos
		);
	}

	// Draw debris
	for( auto& debris : mDebris )
	{
		for( auto& piece : debris.pieces )
		{
			auto const a = piece.rot * piece.pa + piece.pos;
			auto const b = piece.rot * piece.pb + piece.pos;
			auto const c = piece.rot * piece.pc + piece.pos;

			draw_triangle_interp( aSurface,
				a, b, c,
				piece.ca, piece.cb, piece.cc 
			);
		}
	}
}

void AsteroidField::resize( std::uint32_t aWidth, std::uint32_t aHeight )
{
	// WARNING: This is a bit of a hack...
	auto const oldMax = mBoundsMax;

	// Clear debris
	mDebris.clear();

	// New area and asteroid count
	mExactExtent = Vec2f{ float(aWidth), float(aHeight) };

	mBoundsMin = Vec2f{ -mPadding, -mPadding };
	mBoundsMax = mExactExtent + Vec2f{ mPadding, mPadding };

	mActualExtent = mBoundsMax - mBoundsMin;

	float const numAsteroidsf = mActualExtent.x*mActualExtent.y * mDensity;
	std::size_t const numAsteroids = std::size_t(numAsteroidsf+0.5f);

	// Remove asteroids now outside
	std::size_t activeAsteroids = 0;

	auto jt = mShapes.begin();
	for( auto it = mAsteroids.begin(); it != mAsteroids.end(); )
	{
		auto const& aster = *it;

		if( aster.pos.x > mBoundsMax.x || aster.pos.y > mBoundsMax.y )
		{
			it = mAsteroids.erase( it );
			jt = mShapes.erase( jt );
		}
		else
		{
			++it;
			++jt;
			++activeAsteroids;
		}
	}

	mAsteroids.resize( numAsteroids );
	activeAsteroids = std::min( activeAsteroids, numAsteroids );

	mShapes.erase( mShapes.begin()+activeAsteroids, mShapes.end() );
	mShapes.reserve( numAsteroids );

	assert( mShapes.size() == activeAsteroids );

	// Generate new asteroids.
	using Normal_ = std::normal_distribution<float>;
	using Uniform_ = std::uniform_real_distribution<float>;

	if( activeAsteroids < numAsteroids )
	{
		auto dd = mBoundsMax - oldMax;
		if( dd.x < 0.f ) dd.x = 0.f;
		if( dd.y < 0.f ) dd.y = 0.f;

		float xarea = dd.x * mBoundsMax.y;
		float yarea = dd.y * (mBoundsMax.x - dd.x);

		Uniform_ area( 0.f, xarea+yarea );
		Uniform_ xax( oldMax.x, oldMax.x+dd.x );
		Uniform_ xay( 0.f, mBoundsMax.y );
		Uniform_ yax( 0.f, mBoundsMax.x - dd.x );
		Uniform_ yay( oldMax.y, oldMax.y+dd.y );

		Uniform_ angle( 0.f, 2*std::numbers::pi_v<float> );

		Normal_ vvel{ 0.f, mInitialSpeed };
		Normal_ rots{ 0.f, mInitialRot };

		for( std::size_t i = activeAsteroids; i < numAsteroids; ++i )
		{
			Vec2f pos;

			auto const where = area(mRNG);
			if( where <= xarea )
			{
				pos.x = xax( mRNG );
				pos.y = xay( mRNG );
			}
			else
			{
				pos.x = yax( mRNG );
				pos.y = yay( mRNG );
			}

			auto& astr = mAsteroids[i];

			astr.pos = pos;
			astr.vel = Vec2f{ vvel( mRNG ), vvel( mRNG ) };

			astr.rot = make_rotation_2d( angle( mRNG ) );
			astr.radpersec = rots( mRNG );

			// Don't break the speed limits. The space police will get you!
			astr.vel.x = std::clamp( astr.vel.x, -mMaximumSpeed, +mMaximumSpeed );
			astr.vel.y = std::clamp( astr.vel.y, -mMaximumSpeed, +mMaximumSpeed );

			astr.exploding = false;

			// Create shape
			assert( i == mShapes.size() );
			mShapes.emplace_back( make_asteroid( mRNG ) );
		}
	}

	assert( mAsteroids.size() == mShapes.size() );

	compute_asteroid_aabbs_();
}


auto AsteroidField::asteroids() const -> std::span<Asteroid const>
{
	return mAsteroids;
}
std::span<TriangleFan const> AsteroidField::shapes() const
{
	return mShapes;
}


void AsteroidField::explode_( std::size_t aAsteroidIndex )
{
	assert( aAsteroidIndex < mAsteroids.size() );
	auto const& aster = mAsteroids[aAsteroidIndex];
	auto const& shape = mShapes[aAsteroidIndex];

	auto const& fan = shape.vertices();
	auto const& colors = shape.colors();
	assert( fan.size() == colors.size() );

	assert( fan.size() >= 1 );
	auto const center = aster.rot * fan[0];
	auto const color = colors[0];

	std::uniform_real_distribution<float> life( 2.f, 6.f );
	std::uniform_real_distribution<float> mult( 0.8f, 4.f );

	Debris_& debris = mDebris.emplace_back();
	for( std::size_t i = 2; i < fan.size()+1; ++i )
	{
		auto j = i % fan.size();
		if( 0 == j ) j = 1;

		Piece_& piece = debris.pieces.emplace_back();
		piece.pa = center;
		piece.pb = aster.rot * fan[i-1];
		piece.pc = aster.rot * fan[j];

		auto const c = (piece.pa+piece.pb+piece.pc) / 3.f;
		piece.pa -= c;
		piece.pb -= c;
		piece.pc -= c;

		piece.ca = color;
		piece.cb = colors[i-1];
		piece.cc = colors[j];

		piece.pos = aster.pos + c;

		auto const r = c - center;
		piece.vel = mult(mRNG) *(aster.vel + aster.radpersec * Vec2f{ -r.y, r.x });
		piece.rot = make_identity<Mat22f>();
		piece.radpersec = aster.radpersec;

		piece.remainingLife = life( mRNG );
	}
}
void AsteroidField::compute_asteroid_aabbs_()
{
	assert( mAsteroids.size() == mShapes.size() );

	for( std::size_t i = 0; i < mAsteroids.size(); ++i )
	{
		auto& astr = mAsteroids[i];
		auto const& shape = mShapes[i];

		Vec2f min{ std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
		Vec2f max{ std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
		for( auto const& vert : shape.vertices() )
		{
			Vec2f pp = astr.rot * vert + astr.pos;
			min.x = std::min( min.x, pp.x );
			min.y = std::min( min.y, pp.y );
			max.x = std::max( max.x, pp.x );
			max.y = std::max( max.y, pp.y );
		}

		astr.aabbMin = min;
		astr.aabbMax = max;
	}
}
