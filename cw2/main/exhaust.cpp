#include "exhaust.hpp"

#include <random>
#include <algorithm>

#include <draw2d/image.hpp>

#include <vmlib/transform2d.hpp>

Exhaust::Exhaust( RNG& aRNG, std::span<Vec2f const> aEmitters, std::size_t aParticleCount )
	: mMaxCount( aParticleCount )
	, mEmitters( aEmitters.begin(), aEmitters.end() )
	, mRNG( aRNG )
{
	mCooldown = 0.f;
	mSprite = load_image( kSpritePath );

	mParticles.reserve( mMaxCount );
}

Exhaust::~Exhaust() = default;

void Exhaust::update( float aElapsedTimeSec, Vec2f const& aMovement, Vec2f const& aPosition, float aAngleInRadians, bool aAccelerating )
{
	// Should we emit a new puff?
	// Fix at some point: we will always emit at most one per frame, regardless
	// of emitter count. This is also quite frame rate dependent.
	mCooldown -= aElapsedTimeSec;

	if( aAccelerating && mParticles.size() < mMaxCount && mCooldown <= 0.f )
	{
		std::uniform_real_distribution<float> life( 1.f, 1.8f );
		std::uniform_real_distribution<float> cooldown( 0.005f, 0.02f );

		std::uniform_int_distribution<unsigned> emitter( 0, unsigned(mEmitters.size())-1 );
		auto which = emitter(mRNG);

		auto rot = make_rotation_2d( aAngleInRadians );

		auto& part = mParticles.emplace_back();
		part.pos = aPosition + rot * mEmitters[which];
		part.vel = rot * Vec2f{ -10.f, 0.f };
		part.life = life(mRNG);

		mCooldown = cooldown( mRNG );
	}

	// Update existing particles
	for( auto& part : mParticles )
	{
		part.pos += -aMovement + aElapsedTimeSec*part.vel;
		part.life -= aElapsedTimeSec;
	}

	mParticles.erase(
		std::remove_if( mParticles.begin(), mParticles.end(), [] (auto& aPart) {
			return aPart.life <= 0.f;
		} ),
		mParticles.end()
	);
}

void Exhaust::draw( Surface& aSurface ) const
{
	auto const w = int(mSprite->get_width());
	auto const h = int(mSprite->get_height());

	for( auto& part : mParticles )
	{
		blit_blend_subregion( aSurface,
			*mSprite,
			{ 0, 0 },
			{ w, h },
			part.pos
		);
	}
}
