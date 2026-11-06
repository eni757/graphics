#ifndef SAMPLER_HPP_7D47078D_CDC1_4AF0_8E9A_455A46E4D328
#define SAMPLER_HPP_7D47078D_CDC1_4AF0_8E9A_455A46E4D328

// IMPORTANT: DO NOT CHANGE THIS FILE WHEN YOU ARE SOLVING COURSEWORK 2!
//
// For CW2, the sampler.hpp file must remain exactly as it is. In particular,
// you must not change the Sampler2D class interface.

#include <vmlib/stdtypes.hpp>

#include "forward.hpp"
#include "color.hpp"
#include "image.hpp"

class Sampler2D
{
	public:
		// Constructing a Sampler2D is very cheap. No need to hold on to it.
		explicit Sampler2D( ImageRGBA const& );

	public:
		// Sample image with bilinear interpolation
		// 
		// This accepts a coordinate in the range [0,w]x[0,h] as float, i.e.,
		// an "unnormalized" texture coordinate.
		ColorF_Alpha sample( Vec2f aImageCoord ) const;

	private:
		ImageRGBA const* mSrcImage;
};

#include "sampler.inl"
#endif // SAMPLER_HPP_7D47078D_CDC1_4AF0_8E9A_455A46E4D328
