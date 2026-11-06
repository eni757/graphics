/* Inline functions need to have a definition that is visible to the compiler
 * whenever the function is used. They could be defined in the header that
 * declares the function. However, to keep the definitions and declarations
 * somewhat apart, it is a common practice to move them to an "inline" file
 * such as this one (*.inl extension). This file is then #include:ed at the end
 * of the header, to ensure that whoever includes the header also automatically
 * includes the inline file.
 *
 * Inlining allows us to avoid any overheads related to call (when building in
 * "release mode" / with optimizations enabled). This makes functions like
 * set_pixel_srgb() zero overhead abstractions.
 *
 */
inline
void Surface::set_pixel_srgb( Index aX, Index aY, ColorU8_sRGB const& aColor )
{
	assert( aX < mWidth && aY < mHeight ); // IMPORTANT! This line must remain the first line in this function!

	// TODO: your implementation goes here
	Index linear_index = get_linear_index(aX, aY);

	mSurface[linear_index] = aColor.r;
	mSurface[linear_index+1] = aColor.g;
	mSurface[linear_index+2] = aColor.b;
	mSurface[linear_index+3] = 0;





}

inline 
auto Surface::get_width() const noexcept -> Index
{
	return mWidth;
}
inline
auto Surface::get_height() const noexcept -> Index
{
	return mHeight;
}

inline
Rect2F Surface::clip_area() const noexcept
{
	return Rect2F{ 0.f, 0.f, float(mWidth), float(mHeight) };
}

inline
auto Surface::get_linear_index( Index aX, Index aY ) const noexcept -> Index
{
	Index linear_index;

	//we want to go across x y times. then add x
	//multiply by 4, because everything is stored as r, g, b, x. so each pixel is 4 bytes. 4 slots. 

	linear_index = (4*aY) * mWidth + (4*aX);
	
	return linear_index; // TODO: remove this line when you implement this function.
}


inline
ColorU8_sRGB Surface::get_pixel_srgb( Index aX, Index aY ) const noexcept
{
	assert( aX < mWidth && aY < mHeight ); // IMPORTANT! This line must remain the first line in this function!

	return {0, 0, 0}; // TODO: remove this line when you implement this function.
}


