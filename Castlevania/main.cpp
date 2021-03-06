#pragma once
#include"main.h"
#include "StageManager.h"
#include "TilemapManager.h"

Game* game;
SampleKeyHander* keyHandler;
TextureManager* textureManager = TextureManager::getInstance();

//Create keyboard handler for main program
void SampleKeyHander::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	StageManager::getInstance()->onKeyDown(KeyCode);
	//if(KeyCode== DIK_B)
	//{
	//	char* showBoundary = nullptr;
	//	size_t sz = 0;
	//	if (_dupenv_s(&showBoundary, &sz, SHOW_BOUNDARY) == 0 && showBoundary != nullptr)
	//	{
	//		if (strcmp(showBoundary, "1") == 0)
	//			_putenv("SHOW_BOUNDARY=0");
	//		else
	//			_putenv("SHOW_BOUNDARY=1");

	//		free(showBoundary);
	//	}
	//}
}

void SampleKeyHander::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	StageManager::getInstance()->onKeyUp(KeyCode);
}

void SampleKeyHander::KeyState(BYTE* states)
{
	StageManager::getInstance()->keyState(states);
}

/// Create a window then display and running until exit message send
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = createGameWindow(hInstance, nCmdShow,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	loadEnvVariables();
	game = Game::getInstance();
	game->init(hWnd);

	keyHandler = new SampleKeyHander();
	game->initKeyboard(keyHandler);

	loadResources();
	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	run();
}

void loadEnvVariables()
{
	//_putenv("SHOW_BOUNDARY=0");
}

HWND createGameWindow(HINSTANCE hInstance, int nCmdShow,
	int screenWidth, int screenHeight)
{
	WNDCLASSEXW wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.hIconSm = NULL;

	RegisterClassExW(&wc);

	HWND hWnd =
		CreateWindowW(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			screenWidth,
			screenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);
	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		auto now = GetTickCount();
		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render

		auto dt = now - frameStart;
		if (dt >= tickPerFrame)
		{
			frameStart = now;
			game->processKeyboard();
			update(dt);
			render();
		}
		else Sleep(tickPerFrame - dt);
	}
	return 1;
}



// Read data of sprites and animations from file.
void LoadSprites(int id, wstring texName, D3DCOLOR transparentColor = MAIN_TEXTURE_TRANS_COLOR)
{
	std::wstring tex = L"textures\\" + texName + L".png";
	std::wstring sprite_data = L"textures\\" + texName + L"_sprites.txt";
	std::wstring animation_data = L"textures\\" + texName + L"_anims.txt";

	auto textureManager = TextureManager::getInstance();
	textureManager->add(id, tex.c_str(), transparentColor);
	LPDIRECT3DTEXTURE9 texture = textureManager->get(id);

	auto spriteManager = SpriteManager::getInstance();
	auto animationManager = AnimationManager::getInstance();
	ifstream spriteReader, animationReader;

	spriteReader.open(sprite_data);
	animationReader.open(animation_data);

	if (spriteReader.fail())
	{
		DebugOut(L"[ERROR] LoadSprites failed!: ID=%d", id);
		spriteReader.close();
		return;
	}

	if (animationReader.fail())
	{
		DebugOut(L"[ERROR] LoadAnimation failed!: ID=%d", id);
		animationReader.close();
		return;
	}

	// Load sprite data

	string spriteid;
	float left, top, right, bottom;
	float boxleft, boxtop, boxright, boxbottom;

	while (spriteReader >> spriteid >> left >> top >> right >> bottom >> boxleft >> boxtop >> boxright >> boxbottom)
	{
		spriteManager->add(spriteid, left, top, right, bottom, boxleft, boxtop, boxright, boxbottom, texture);
	}

	spriteReader.close();

	// Load animation data

	string animationId;
	string line;
	string spriteId;
	int frameTime;

	while (getline(animationReader, line))
	{
		auto ani = new Animation();

		istringstream iss(line, istringstream::in);
		iss >> animationId;

		while (iss >> spriteId >> frameTime)
			ani->add(spriteId, frameTime);

		animationManager->add(animationId, ani);
	}

	animationReader.close();
}

void loadTextures()
{
	ifstream reader;

	reader.open(L"init-data\\textures.txt");
	if (reader.fail())
	{
		DebugOut(L"[ERROR] LoadSprites failed!: ID");
		reader.close();
		return;
	}

	string name;
	int r, id, g, b;
	while (reader >> id >> name >> r >> g >> b)
	{
		std::wstring wsTemp(name.begin(), name.end());
		LoadSprites(id, wsTemp, D3DCOLOR_XRGB(r, g, b));
	}
	reader.close();
}

void loadMaps()
{
	ifstream tileMapReader, gridReader;

	tileMapReader.open(L"init-data\\maps.txt");
	if (tileMapReader.fail()) {
		DebugOut(L"[ERROR] load maps failed!: ID");
		tileMapReader.close();
		return;
	}
	gridReader.open(L"init-data\\grids.txt");
	if (gridReader.fail()) {
		DebugOut(L"[ERROR] load gridReader failed!: ID");
		gridReader.close();
		return;
	}

	int id, mapWidth, mapHeight, tileWidth, tileHeight;
	string stageName, tileMapName;
	vector<TileMapInfo> mapInfos;
	while (tileMapReader >> id >> tileMapName >> stageName >> mapWidth >> mapHeight >> tileWidth >> tileHeight) {
		std::wstring tileMapNameWS(tileMapName.begin(), tileMapName.end());
		mapInfos.push_back({ id,tileMapNameWS,stageName,mapWidth,mapHeight,tileWidth,tileHeight });
	}

	vector <GridInfo> gridInfos;
	int mapId, gridWidth, gridHeight, cellW, cellH;
	string gridName;
	while (gridReader >> gridName >> gridWidth >> gridHeight >> cellW >> cellH) {
		std::wstring wsGrid(gridName.begin(), gridName.end());
		gridInfos.push_back({ wsGrid,gridWidth,gridHeight,cellW,cellH });
	}

	auto stages = StageManager::getInstance();
	stages->init(mapInfos, gridInfos);

	tileMapReader.close();
	gridReader.close();

}

void loadResources()
{
	textureManager->add(ID_TEX_BBOX, L"textures\\Bbox.png");
	loadTextures();
	loadMaps();
}

void update(DWORD dt)
{
	StageManager::getInstance()->update(dt);
}

void render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->getDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->getBackBuffer();
	LPD3DXSPRITE spriteHandler = game->getSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		auto bg = BACKGROUND_COLOR;

		if (StageManager::getInstance()->isDoingThunderEffect()) {
			auto random= rand() % 100;
			bg = random > 60 ? THUNDER_EFFECT_COLOR : BACKGROUND_COLOR;
		}
		d3ddv->ColorFill(bb, NULL, bg);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		float x, y;
		Game::getInstance()->getCameraPosition(x, y);

		StageManager::getInstance()->render();
		spriteHandler->End();
		d3ddv->EndScene();
	}
	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}