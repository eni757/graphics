#include <draw2d/image.hpp>

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cassert>

#include <stb_image.h>

#include <draw2d/surface.hpp>
#include <support/error.hpp>

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( Index, Index, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
		throw Error( "Unable to load image \"{}\"", aPath );

	return std::make_unique<STBImageRGBA_>(
		ImageRGBA::Index(w),
		ImageRGBA::Index(h),
		ptr
	);
}

// Task 1.4
void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	// TODO: your implementation goes here

	/* 
	draw image on the surface. 
	we need to check that the alpha value is more than 128
	make sure the image is centred (so add 0.5 to the position)

	*/

	ColorU8_sRGB_Alpha colour_from_image;
	ColorU8_sRGB colour_to_screen;
	int ImageWidth = aImage.get_width();
	int ImageHeight = aImage.get_height();

	int screenWidth = aSurface.get_width();
	int screenHeight = aSurface.get_height();


	//want to start at the bottom corner and check if bottom corner is in the surface image
	
	int bottomCornerx = aPosition.x - (ImageWidth/2);
	int bottomCornery = aPosition.y - (ImageHeight/2);

	//min x is either bottom corner, or 0 or screen width. 
	int min_x = bottomCornerx;
	//min y is either bottom corner, or 0 or screen height
	int min_y = bottomCornery;

	int offset_x = 0;  //offset is difference between bottom corner and min x
	 int offset_y = 0;

	if (min_x < 0) {
		min_x = 0;
		offset_x = min_x - bottomCornerx;
	}

	//if x is on the screen then offset is 0.
	

	// //max x is either top corner of image, 0, or screen width
	// int max_x = aPosition.x + (ImageWidth/2);
	// //max y is either top corner of image, 0, or screen height
	// int max_y = aPosition.y + (ImageHeight/2);



	// //check if min_x and min_y is out of bounds
	// if (min_x < 0) {min_x = 0;}
	// if (min_y < 0) {min_y = 0;}

	// if (min_x >= screenWidth) {min_x = screenWidth -1;}
	// if(min_y >= screenHeight) {min_y = screenHeight -1;}

	// //do the same for max x and max y
	// if (max_x>= screenWidth) {max_x = screenWidth-1;} 
	// if (max_x < 0) {max_x = 0;}
	
	// if (max_y>= screenHeight) {max_y = screenHeight-1;}  
	// if (max_y < 0) {max_y = 0;}

	

	// printf("max x: %d, max y: %d\n", max_x, max_y);
	// printf("min x: %d, min y: %d\n", min_x, min_y);
	// // printf("position x: %f\n", aPosition.x);
	// // printf("position y: %f\n", aPosition.y);
	// printf("bottom corner x: %d, bottom corner y: %d\n", bottomCornerx, bottomCornery);
	// printf("offset x: %d, offset y: %d\n", offset_x, offset_y);

	
	// int i, j;
	// for (i=min_x;i < max_x; i++) {
	// 	if( offset_x + 0.5f>=  screenWidth ||  offset_x + 0.5f<=0) {
	// 				continue;
	// 			}
	// //	printf("drawing y: %d" , j);
	// 	for(j = min_y;j < max_y; j++) {
			
	// 		//if its more than 128, we can set the pixel to the colour. if not then move on

	// 		if(offset_y >= screenWidth || offset_y <= 0) {
	// 			continue;;
	// 		}
			
	// 		printf("image height: %d, image width: %d", offset_y, offset_x);
	// 		colour_from_image = aImage.get_pixel(offset_x, offset_y);

	// 		if( colour_from_image.a >= 128) {
	// 			//convert colour from sRGBa to sRGB

	// 			colour_to_screen.r = colour_from_image.r;
	// 			colour_to_screen.g = colour_from_image.g;
	// 			colour_to_screen.b = colour_from_image.b;

			
	// 			if(bottomCornery + j +0.5f  >= screenHeight || j <=0) {
	// 				continue;
	// 			}
	// 			printf("hello");

	// 			aSurface.set_pixel_srgb(i+0.5f, j+0.5f, colour_to_screen);
	// 			offset_y += 1;
	// 		}
	// 		offset_x += 1;


	// 	}

	// }

	

}

// Task 2.1
void blit_masked_subregion( Surface& aSurface, ImageRGBA const& aImage, Vec2i aSourceOrigin, Vec2i aSourceSize, Vec2f aPosition )
{
	// TODO: your implementation goes here
	


}

// Task 2.2
void blit_blend_subregion( Surface& aSurface, ImageRGBA const& aImage, Vec2i aSourceOrigin, Vec2i aSourceSize, Vec2f aPosition )
{
	// TODO: your implementation goes here
}


namespace
{
	STBImageRGBA_::STBImageRGBA_( Index aWidth, Index aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}
