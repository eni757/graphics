#include <algorithm>
#include <cstdio>

inline
bool intersect_circle_aabb( Vec2f aCircleCenter, float aRadius, Vec2f aAABBMin, Vec2f aAABBMax ) noexcept
{

	//clamp  so 

	float q_x = std::min(std::max(aCircleCenter.x, aAABBMin.x), aAABBMax.x);
	float q_y = std::min(std::max(aCircleCenter.y, aAABBMin.y), aAABBMax.y); 

	printf("qx: %f, qy: %f", q_x, q_y);

	//float check = fabs(C - Q). pow(check, 2)
	
	float condition_x = pow(fabs(aCircleCenter.x - q_x), 2);
	float condition_y = pow(fabs(aCircleCenter.y - q_y), 2);

	printf("x: %f, y: %f", condition_x, condition_y);

	float rad_squared = pow(aRadius, 2);

	//make into one length?
	float length = pow(condition_x, 2) + pow(condition_y, 2);

	printf("length before this: %f", length);
	length = pow(length, 0.5);

	printf("length: %f\nradius: %f\n", length, rad_squared);

	if (length <= rad_squared) {
		return true;
	}




	//asteroids are also circles i assume?


	return false; // TODO: implement me
}




#include <draw2d/intersect.hpp>

#include <algorithm>

bool intersect_circle_tri( Vec2f aCircleCenter, float aRadius, Vec2f aP0, Vec2f aP1, Vec2f aP2 ) noexcept
{
	
	//want to check if this the any point in the circle is in the triangle. 
	int minx = aCircleCenter.x  - aRadius;
	int miny = aCircleCenter.y - aRadius;
	int maxx = aCircleCenter.x + aRadius;
	int maxy = aCircleCenter.y + aRadius;
	Vec2f p0p, p1p, p2p, point;
	float area_A, area_B, area_C;
	float area = fabs(aP0.x * aP1.y - aP1.x * aP0.y) / 2;
	float alpha, beta, gamma, total;

	for (int i = minx; i <= maxx; i++) {
		
		for (int j = miny; j <= maxy; j++) {

			point = {float(i), float(j)};
			//check if point is in the 

			p0p = operator-(aP0, point);
			p2p = operator-(aP2, point);
			area_A = fabs(( p0p.x * p2p.y) - (p2p.x * p0p.y)) /2;
			alpha = area_A / area;


				if( alpha <= 1 && alpha >= 0) { 
				alpha = area_A / area;	
			 	p1p = operator-(aP1, point);
				area_B =fabs(( p0p.x * p1p.y) - (p1p.x *p0p.y)) /2;
				beta = area_B / area;


					if (beta <= 1 && beta >= 0) { 
						area_C = fabs(( p1p.x * p2p.y) - (p2p.x *p1p.y)) /2;
						gamma = area_C / area;

						if (gamma <= 1 && gamma >= 0) { 
						//printf("total: %f", total);
						total = alpha+beta+gamma-1;
						//for floating point rounding errors. 
						if (total< 0.000001 ) {
							//point is in the triangle. 

							return true;

						}
						}

					}
			}
			
		}
	}

	
	
	return false; // TODO: implement me
}



#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <print>
#include <random>
#include <typeinfo>
#include <stdexcept>

#include <draw2d/surface.hpp>
#include <draw2d/draw.hpp>
#include <draw2d/shape.hpp>
#include <draw2d/intersect.hpp>

#include <support/error.hpp>
#include <support/context.hpp>
#include <support/runconfig.hpp>

#include <vmlib/stdtypes.hpp>
#include <vmlib/transform2d.hpp>

#include "defaults.hpp"
#include "state.hpp"
#include "exhaust.hpp"
#include "spaceship.hpp"
#include "background.hpp"
#include "asteroid_field.hpp"


namespace
{
	constexpr char const* kWindowTitle = "COMP3812-Coursework 2";

	void glfw_callback_error_( int, char const* );

	void glfw_callback_key_( GLFWwindow*, int, int, int, int );
	void glfw_callback_button_( GLFWwindow*, int, int, int );
	void glfw_callback_motion_( GLFWwindow*, double, double );

	struct GLFWCleanupHelper
	{
		~GLFWCleanupHelper();
	};
	struct GLFWWindowDeleter
	{
		~GLFWWindowDeleter();
		GLFWwindow* window;
	};
}

int main( int aArgc, char* aArgv[] ) try
{
	// Parse command line arguments
	RuntimeConfig const config = parse_command_line( aArgc, aArgv );

	// Initialize GLFW
	if( GLFW_TRUE != glfwInit() )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwInit() failed with '{}' ({})", msg, ecode );
	}

	// Ensure that we call glfwTerminate() at the end of the program.
	GLFWCleanupHelper cleanupHelper;

	glfwSetErrorCallback( &glfw_callback_error_ );

	glfwWindowHint( GLFW_SRGB_CAPABLE, GLFW_TRUE );
	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

	glfwWindowHint( GLFW_SCALE_FRAMEBUFFER, GLFW_FALSE ); // Disable content scaling for now (e.g., retina displays)

#	if !defined(__APPLE__)
	// Most platforms will support OpenGL 4.3
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
#	else // defined(__APPLE__)
	// Apple has at most OpenGL 4.1, so don't ask for something newer.
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
#	endif // ~ __APPLE__
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#	if !defined(NDEBUG)
	// When building in debug mode, request an OpenGL debug context. This
	// enables additional debugging features. However, this can carry extra
	// overheads. We therefore do not do this for release builds.
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#	endif // ~ !NDEBUG

	GLFWwindow* window = glfwCreateWindow(
		config.initialWindowWidth,
		config.initialWindowHeight,
		kWindowTitle,
		nullptr, nullptr
	);

	if( !window )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwCreateWindow() failed with '{}' ({})", msg, ecode );
	}

	GLFWWindowDeleter windowDeleter{ window };

	// Runtime state
	State state;

	state.crosshair = glfwCreateStandardCursor( GLFW_CROSSHAIR_CURSOR );

	// Set up event handling
	glfwSetWindowUserPointer( window, &state );

	glfwSetKeyCallback( window, &glfw_callback_key_ );
	glfwSetMouseButtonCallback( window, &glfw_callback_button_ );
	glfwSetCursorPosCallback( window, &glfw_callback_motion_ );

	// Set up drawing stuff
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 ); // V-Sync is on.

	// Actual framebuffer size
	// With optional fbshift scaling
	int iwidth, iheight;
	glfwGetFramebufferSize( window, &iwidth, &iheight );

	assert( iwidth >= 0 && iheight >= 0 );
	auto fbwidth = std::uint32_t(iwidth) >> config.framebufferScaleShift;
	auto fbheight = std::uint32_t(iheight) >> config.framebufferScaleShift;

	// Resources
	RNG rng( std::random_device{}() );

	Context context( fbwidth, fbheight );
	Surface surface( fbwidth, fbheight );

	Background background( rng, fbwidth, fbheight );
	AsteroidField asteroids( rng, fbwidth, fbheight );

	auto const spaceship = make_spaceship_atlas();

	Exhaust shipExhaust( rng, spaceship.kNozzles );

	// Main loop
	auto lastUpdateTime = Clock::now();

	while( !glfwWindowShouldClose( window ) )
	{
		// Check if window was resized.
		{
			int nwidth, nheight;
			glfwGetFramebufferSize( window, &nwidth, &nheight );

			if( 0 == nwidth || 0 == nheight )
			{
				// Window minimized? Pause until it is unminimized.
				// This is a bit of a hack.
				do
				{
					glfwWaitEvents();
					glfwGetFramebufferSize( window, &nwidth, &nheight );
				} while( 0 == nwidth || 0 == nheight );
			}

			if( iwidth != nwidth || iheight != nheight )
			{
				iwidth = nwidth;
				iheight = nheight;

				glViewport( 0, 0, iwidth, iheight );

				assert( iwidth >= 0 && iheight >= 0 );
				fbwidth = std::uint32_t(iwidth) >> config.framebufferScaleShift;
				fbheight = std::uint32_t(iheight) >> config.framebufferScaleShift;

				// Resize things
				surface = Surface( fbwidth, fbheight );

				context.resize( fbwidth, fbheight );
				background.resize( fbwidth, fbheight );
				asteroids.resize( fbwidth, fbheight );
			}
		}

		// Update state
		auto const now = Clock::now();
		auto const dt = std::chrono::duration_cast<Secondsf>(now - lastUpdateTime).count();
		lastUpdateTime = now;

		state_update( state, dt );

		auto const offs = Vec2f{ fbwidth*0.5f, fbheight*0.5f };

		background.update( state.player.position, state.thisFrame.movement );
		asteroids.update( state.thisFrame.dt, state.thisFrame.movement );
		shipExhaust.update( state.thisFrame.dt, state.thisFrame.movement, offs, state.player.angle, state.player.accelerationMagnitude > 0.f );

		// Draw scene
		surface.clear();


		background.draw( surface );
		asteroids.draw( surface );

		std::span<const AsteroidField::Asteroid> numAsteroids = asteroids.asteroids(); 

		for (int i = 0; i < (int)numAsteroids.size(); i++) {

			if (intersect_circle_aabb(numAsteroids[i].pos, spaceship.kRadius, numAsteroids[i].aabbMin, numAsteroids[i].aabbMax )) {

				asteroids.explode(i);
			}
			
			
			
		}

		//asteroid ; 
		

		shipExhaust.draw( surface );

		spaceship.draw( surface, state.player.angle, offs );

		context.draw( surface );

		// Display results
		glfwSwapBuffers( window );

		// Let GLFW process events
		// Fix for 2026/27: This is moved to the end of the frame, to work 
		// around a Wayland issue (no initial events delivered, so the first
		// frame would never draw).
		glfwPollEvents();
	}

	// Cleanup.
	// For now, all objects are automatically cleaned up when they go out of
	// scope.
	
	return 0;
}
catch( std::exception const& eErr )
{
	std::print( stderr, "Top-level Exception ({}):\n", typeid(eErr).name() );
	std::print( stderr, "{}\n", eErr.what() );
	std::print( stderr, "Bye.\n" );
	return 1;
}


namespace
{
	void glfw_callback_error_( int aErrNum, char const* aErrDesc )
	{
		std::print( stderr, "GLFW error: {} ({})\n", aErrDesc, aErrNum );
	}

	void glfw_callback_key_( GLFWwindow* aWindow, int aKey, int, int aAction, int )
	{
		if( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction )
		{
			glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
			return;
		}

		auto* state = static_cast<State*>(glfwGetWindowUserPointer( aWindow ));
		assert( state );

		if( EInputMode::standard == state->inputMode )
		{
			if( GLFW_KEY_SPACE == aKey && GLFW_PRESS == aAction )
			{
				state->inputMode = EInputMode::piloting;
				glfwSetCursor( aWindow, state->crosshair );
			}
		}
		else if( EInputMode::piloting == state->inputMode )
		{
			if( GLFW_KEY_SPACE == aKey && GLFW_PRESS == aAction )
			{
				state->inputMode = EInputMode::standard;
				glfwSetCursor( aWindow, nullptr );
			}
		}
	}

	void glfw_callback_button_( GLFWwindow* aWindow, int aBut, int aAct, int )
	{
		auto* state = static_cast<State*>(glfwGetWindowUserPointer( aWindow ));
		assert( state );

		if( EInputMode::piloting == state->inputMode )
		{
			if( GLFW_MOUSE_BUTTON_RIGHT == aBut )
			{
				if( GLFW_PRESS == aAct )
					state->player.accelerationMagnitude = 500.f;
				else if( GLFW_RELEASE == aAct )
					state->player.accelerationMagnitude = 0.f;
			}
		}
	}

	void glfw_callback_motion_( GLFWwindow* aWindow, double aX, double aY )
	{
		auto* state = static_cast<State*>(glfwGetWindowUserPointer( aWindow ));
		assert( state );

		int iwidth, iheight;
		glfwGetFramebufferSize( aWindow, &iwidth, &iheight );

		if( EInputMode::piloting == state->inputMode )
		{
			Vec2f relative{ float(aX) - iwidth/2.f, iheight/2.f - float(aY) };
			state->player.angle = std::atan2( relative.y, relative.x );
		}
	}
}

namespace
{
	GLFWCleanupHelper::~GLFWCleanupHelper()
	{
		glfwTerminate();
	}

	GLFWWindowDeleter::~GLFWWindowDeleter()
	{
		if( window )
			glfwDestroyWindow( window );
	}
}


