#include <draw2d/intersect.hpp>

#include <algorithm>

bool intersect_circle_tri( Vec2f aCircleCenter, float aRadius, Vec2f aP0, Vec2f aP1, Vec2f aP2 ) noexcept
{
	
	//find closest point on each edge to the circle centre

	//check if triangle is inside cirlce
	//find closest edge of p0p1
	//check distance against radius
	//find closest edge of p1p2
	//check distance against radius
	//find closest edge of p0p2
	//chek distance against radius

	//t = C - O . d / d.d


	//the d values (vectors)
	Vec2f d_P0P1 = {aP0.x - aP1.x, aP0.y - aP1.y};
	Vec2f d_P1P2 = {aP1.x - aP2.x, aP1.y - aP2.y};
	Vec2f d_P0P2 = {aP0.x - aP2.x, aP0.y - aP2.y};

	//C - Origin
	Vec2f p1origin = {aCircleCenter.x - aP1.x, aCircleCenter.y - aP1.y};
	Vec2f p2origin = {aCircleCenter.x - aP2.x, aCircleCenter.y - aP2.y};
	
	//t closest
	float t_p0p1 =( p1origin.x * d_P0P1.x + p1origin.y * d_P0P1.y) / (d_P0P1.x * d_P0P1.x + d_P0P1.y * d_P0P1.y);
	float t_p0p2 = (p2origin.x * d_P0P2.x + p2origin.y * d_P0P2.y) / (d_P0P2.x * d_P0P2.x + d_P0P2.y * d_P0P2.y);
	float t_p1p2 = (p2origin.x * d_P1P2.x + p2origin.y * d_P1P2.y) / (d_P1P2.x * d_P1P2.x + d_P1P2.y * d_P1P2.y);

	//t has to be between 0 and 1 so
	t_p0p1 = std::clamp(t_p0p1, 0, 1);
	t_p0p2 = std::clamp(t_p0p2, 0, 1);
	t_p1p2 = std::clamp(t_p1p2, 0, 1);

	Vec2f q_1 = {aP1.x + t_p0p1*d_P0P1.x, aP1.y + t*d_P0P1.y};
	Vec2f q_2 = {aP2.x + t_p0p2*d_P0P2.x, aP2.y + t*d_P0P2.y};
	Vec2f q_3 = {aP2.x + t_p1p2*d_P1P2.x, aP2.y + t*d_P1P2.y};


	float distance_x_1 = aCircleCenter.x - q_1.x;
	float distance_y_1 = aCircleCenter.y - q_1.y;
	float distance_x_2 = aCircleCenter.x - q_2.x;
	float distance_y_2 = aCircleCenter.y - q_2.y;
	float distance_x_3 = aCircleCenter.x - q_3.x;
	float distance_y_3 = aCircleCenter.y - q_3.y;

	float condition_1 = distance_x_1 * distance_x_1 + distance_y_1 * distance_y_1;
	float condition_2 = distance_x_2 * distance_x_2 + distance_y_2 * distance_y_2;
	float condition_3 = distance_x_3 * distance_x_3 + distance_y_3 * distance_y_3;

	float rad_squared = aRadius * aRadius;

	if (condition_1 <= rad_squared || condition_2 <=rad_squared || condition_3 <= rad_squared) {
		return true;
	}

	
	return false; // TODO: implement me
}



#include <algorithm>
#include <cstdio>

inline
bool intersect_circle_aabb( Vec2f aCircleCenter, float aRadius, Vec2f aAABBMin, Vec2f aAABBMax ) noexcept
{

	//clamp  so 

	

	float q_x = std::clamp(aCircleCenter.x, aAABBMin.x, aAABBMax.x);
	float q_y = std::clamp(aCircleCenter.y, aAABBMin.y, aAABBMax.y);



	// printf("aAABBMin: %f, %f\n", aAABBMin.x, aAABBMin.y);
	// printf("aAABBMax: %f, %f\n", aAABBMax.x, aAABBMax.y);
	


	float centre = aCircleCenter.x * aCircleCenter.x + aCircleCenter.y * aCircleCenter.y;
//	centre = pow(centre, 0.5);
	float q = q_x * q_x + q_y * q_y;
//	q = pow(q, 0.5);

	float d_x = aCircleCenter.x - q_x;
	float d_y = aCircleCenter.y - q_y;

	float condition = d_x * d_x + d_y * d_y;

//
	//printf("centre: %f", centre);
//
	//float check = fabs(C - Q). pow(check, 2)
	
//	float condition = (centre - q) * (centre - q);


	// printf("q: %f\n", q);
	 //printf("condition: %f\n", condition);

	float rad_squared = aRadius * aRadius;

	//make into one length?

	//printf("radius: %f\n", rad_squared);

	if (condition <= rad_squared) {
		// printf("radius: %f\n", rad_squared);
		// printf("condition: %f\n", condition);
		// printf("circle centre: %f, %f\n", aCircleCenter.x, aCircleCenter.y);
		// printf("qx: %f, qy: %f\n", q_x, q_y);
		return true;
	}




	//asteroids are also circles i assume?


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

	// float minShip_x = state.player.position.x - spaceship.kRadius;
	// float maxShip_x = state.player.position.x + spaceship.kRadius;
	// float minShip_y = state.player.position.y - spaceship.kRadius;
	// float maxShip_y = state.player.position.y + spaceship.kRadius;

	// Vec2f aabbMin = {minShip_x, minShip_y};
	// Vec2f aabbMax = {maxShip_x, maxShip_y};

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

//		std::span<const AsteroidField::Asteroid> asteriodList = asteroids.asteroids(); 
		auto asteroidList = asteroids.asteroids();
		auto asteroidShapes = asteroids.shapes();

		for (std::size_t i = 0; i < asteroidList.size(); i++) {

			//printf("%d", intersect_circle_aabb(state.player.position, spaceship.kRadius, numAsteroids[i].aabbMin, numAsteroids[i].aabbMax));

			auto asteroid = asteroidList[i];
			auto shape = asteroidShapes[i];
			

			if (intersect_circle_aabb(offs, spaceship.kRadius, asteroid.aabbMin, asteroid.aabbMax )) {


				auto vertices = shape.vertices();
				auto vertex_centre = asteroid.rot * vertices[0] + asteroid.pos;

				for (std::size_t j = 1; j < vertices.size() - 1; j++) {

					auto vertex_1 = asteroid.rot * vertices[j] + asteroid.pos; //like in the aabbs function somewhere
					auto vertex_2 = asteroid.rot * vertices[j+1] + asteroid.pos;

					if (intersect_circle_tri(offs, spaceship.kRadius, vertices[0], vertices[j], vertices[j+1])) {
					
						asteroids.explode(i);
						break;

					}
				}
				

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


