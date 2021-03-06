#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <dinput.h>

class KeyboardHandler
{
public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
};
