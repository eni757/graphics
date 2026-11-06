#include <draw2d/image.hpp>

inline
Sampler2D::Sampler2D( ImageRGBA const& aImage )
	: mSrcImage( &aImage )
{}

inline
ColorF_Alpha Sampler2D::sample( Vec2f aImageCoord ) const
{
	return {0.f, 0.f, 0.f, 0.f}; // TODO: implement me
}
