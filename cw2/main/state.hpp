#ifndef STATE_HPP_79F3272C_6103_46D8_AA3D_6EBE1F6466B7
#define STATE_HPP_79F3272C_6103_46D8_AA3D_6EBE1F6466B7

#include <glad/glad.h> // Make sure <GL/gl.h> isn't being included.
#include <GLFW/glfw3.h>

#include <vmlib/stdtypes.hpp>

enum class EInputMode
{
	standard,
	piloting
};

struct State
{
	EInputMode inputMode = EInputMode::standard;

	// "player" data
	struct Player_
	{
		float angle = 0.f;
		Vec2f velocity = { 0.f, 0.f };
		Vec2f position = { 0.f, 0.f };

		float accelerationMagnitude = 0.f;
	} player;

	// Frame data
	struct Frame_
	{
		float dt = 0.f;
		Vec2f movement = { 0.f, 0.f };
	} thisFrame;

	// Misc.
	GLFWcursor* crosshair = nullptr;
};


void state_update( State&, float aDeltaSeconds );

#endif // STATE_HPP_79F3272C_6103_46D8_AA3D_6EBE1F6466B7
