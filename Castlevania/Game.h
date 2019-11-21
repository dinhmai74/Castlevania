#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <unordered_map>
#include"debug.h"
#include <dinput.h>
#include <vector>
#include "KeyboardHandler.h"
#include "Box.h"
#include "Camera.h"

#define KEYBOARD_BUFFER_SIZE 1024
#define DIRECTINPUT_VERSION 0x0800
#define SCREEN_LEFT 0

constexpr auto WINDOW_CLASS_NAME = LR"(Castlevania)";
constexpr auto MAIN_WINDOW_TITLE = LR"(Castlevania)";

constexpr auto SCREEN_WIDTH = 512;
constexpr auto SCREEN_HEIGHT = 480;

constexpr auto MAX_FRAME_RATE = 120;
#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0) //black
//#define BACKGROUND_COLOR D3DCOLOR_XRGB(255,255,255) //white
//#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)


class Game
{
private:
	static Game* instance;
	HWND hWnd; // Window handle

	LPDIRECT3D9 d3d = NULL; // direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;// Direct 3d device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;  	// Sprite helper library to help us draw 2D image on the screen

	LPDIRECTINPUT8       di;		// The DirectInput object
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device
	BYTE  keyStates[256];			// DirectInput keyboard state buffer
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data
	KeyboardHandler* keyHandler;

	Camera* camera;
	
	ID3DXFont* font;

public:
	void init(HWND hWnd);
	void draw(int nx, float x, float y, LPDIRECT3DTEXTURE9 texture, Box frameRect, Box boundaryRect, int alpha, int r = 255, int g = 255, int b = 255);
	void draw(int accordingCam, int nx, float x, float y, LPDIRECT3DTEXTURE9 texture, Box frameRect, Box boundaryRect, int alpha, int r = 255, int g = 255, int b = 255);

	static Game* getInstance() {
		if (instance == NULL) instance = new Game();
		return instance;
	};

	// handle direct x
	LPDIRECT3DDEVICE9 getDirect3DDevice() const { return this->d3ddv; }
	LPDIRECT3DSURFACE9 getBackBuffer() const { return backBuffer; }
	LPD3DXSPRITE getSpriteHandler() const { return this->spriteHandler; }

	void initKeyboard(KeyboardHandler* handler);
	int isKeyDown(int keyCode);
	int isKeyUp(int keyCode);
	void processKeyboard();
	void setCameraPosition(float x, float y);
	void getCameraPosition(float& x, float& y);
	D3DXVECTOR2 getCameraPosition() const { return { getCamera()->getX() ,getCamera()->getY()}; }
	ID3DXFont* getFont() const { return this->font; }
	Region getLimitCamX() const { return getCamera()->getLimitX(); }
	void setLimitCamX(Region val) { getCamera()->setLimitX(val); }

	~Game();
	Camera* getCamera() const { return Camera::getInstance(); }
};
