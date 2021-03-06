﻿#include "Game.h"
Game* Game::instance = NULL;

void Game::init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugStringW(L"[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);
	// Font
	font = NULL;
	AddFontResourceExW(L"Font\\prstart.ttf", FR_PRIVATE, NULL);

	HRESULT hr = D3DXCreateFontW(
		getDirect3DDevice(), 16, 0, FW_NORMAL, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"Press Start", &font);

	camera = Camera::getInstance();

	OutputDebugStringW(L"[INFO] InitGame done;\n");
}

void Game::draw(int nx, float x, float y, LPDIRECT3DTEXTURE9 texture, Box frameRect, Box boundaryRect, int alpha, int r, int g, int b)
{
	draw(1, nx, x, y, texture, frameRect, boundaryRect, alpha, r, g, b);
}

void Game::draw(int accordingCam, int nx, float x, float y, LPDIRECT3DTEXTURE9 texture, Box frameRect, Box boundaryRect,
	int alpha, int r, int g, int b)
{
	if (!texture || !spriteHandler)
	{
		DebugOut(L"[INFO]sprite has problem!\n");
		return;
	};
	// calculation position of object in game
	// if according =0 => don't move base on cam
	D3DXVECTOR3 p(floor(x - getCamera()->getX()* accordingCam), floor(y - getCamera()->getY()* accordingCam), 0);

	// flip sprite, using nx parameter
	D3DXMATRIX oldTransform;
	D3DXMATRIX newTransform;

	spriteHandler->GetTransform(&oldTransform);

	D3DXVECTOR2 center;
	D3DXVECTOR2 rotate = D3DXVECTOR2(-nx, 1);

	RECT convertFrameRect;
	convertFrameRect.top = frameRect.t;
	convertFrameRect.right = frameRect.r;
	convertFrameRect.left = frameRect.l;
	convertFrameRect.bottom = frameRect.b;

	center.x = p.x + (frameRect.r - frameRect.l) / 2;
	center.y = p.y + (frameRect.b - frameRect.t) / 2;

	// Xây dựng một ma trận 2D lưu thông tin biến đổi (scale, rotate)
	D3DXMatrixTransformation2D(&newTransform, &center, 0.0f, &rotate, NULL, 0.0f, NULL);

	// Cần nhân với ma trận cũ để tính ma trận biến đổi cuối cùng
	D3DXMATRIX finalTransform = newTransform * oldTransform;
	spriteHandler->SetTransform(&finalTransform);

	spriteHandler->Draw(texture, &convertFrameRect, NULL, &p, D3DCOLOR_ARGB(alpha, r, g, b));

	spriteHandler->SetTransform(&oldTransform);
}

void Game::processKeyboard()
{
	if (keyHandler == nullptr)
	{
		DebugOut(L"Cannot connect keyboard device\n");
		return;
	}
	HRESULT hr;

	// Collect all key states first
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h == DI_OK)
			{
				DebugOut(L"[INFO] Keyboard re-acquired!\n");
			}
			else return;
		}
		else
		{
			DebugOut(L"[ERROR] DINPUT::GetDeviceState failed. Error: %d\n", hr);
			return;
		}
	}
	if (keyHandler != nullptr) keyHandler->KeyState((BYTE*)&keyStates);
	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	// Scan through all buffered events, check if the key is pressed or released
	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			keyHandler->OnKeyDown(KeyCode);
		else
			keyHandler->OnKeyUp(KeyCode);
	}
}

void Game::setCameraPosition(float x, float y)
{
	getCamera()->setPos({x,y});
}

void Game::getCameraPosition(float& x, float& y)
{
	x = getCamera()->getX();
	y = getCamera()->getY();
}

int Game::isKeyDown(int keyCode)
{
	return (keyStates[keyCode] & 0x80) > 0;
}

int Game::isKeyUp(int keyCode)
{
	return (keyStates[keyCode] & 0x80) > 0;
}

void Game::initKeyboard(KeyboardHandler* handler)
{
	HRESULT
		hr = DirectInput8Create
		(
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);

	if (hr != DI_OK)
	{
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);
	if (hr != DI_OK)
	{
		return;
	}

	hr = didv->SetDataFormat(&c_dfDIKeyboard);
	hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		return;
	}

	this->keyHandler = handler;
}

Game::~Game()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}