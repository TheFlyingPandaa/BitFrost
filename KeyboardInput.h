#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "DirectX/include/dinput.h"
#include <DirectXMath.h>

#pragma comment (lib, "DirectX/lib/dinput8.lib")

using namespace DirectX;

class KeyboardInput
{
public:
	KeyboardInput();
	~KeyboardInput();

	void InitKeyboardInput(HINSTANCE & hInstance, HWND & wndHandle);
	void GameInput(DIMOUSESTATE & mouseLastState);

	const XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	


private:
	IDirectInputDevice8 * DIKeyboard;
	IDirectInputDevice8* DIMouse;

	LPDIRECTINPUT8 DirectInput;
	
	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;
	float camYaw = 0.0f;
	float camPitch = 0.0f;

public:
	float getMoveLeftRight() const
	{
		return moveLeftRight;
	}

	void setMoveLeftRight(float move_left_right)
	{
		moveLeftRight = move_left_right;
	}

	float getMoveBackForward() const
	{
		return moveBackForward;
	}

	void setMoveBackForward(float move_back_forward)
	{
		moveBackForward = move_back_forward;
	}

	float getCamYaw() const
	{
		return camYaw;
	}

	float getCamPitch() const
	{
		return camPitch;
	}
};

#endif

