#pragma once
#include <Windows.h>
#include "Simon.h"
#include "Boundary.h"
#include "Grid.h"

class Stage
{

public:
	Stage();
	~Stage();

	void init(int mapId, wstring mapName);
	void render();
	void update(DWORD dt);
	void updateGrid();
	void onKeyDown(int keyCode);
	void onKeyUp(int keyCode);
	void keyState(BYTE* states);
private:
	Simon* simon{};
	int mapId;
	Grid* grid;
	wstring mapName;
	vector<GameObject*> listBoundary;
	vector<GameObject*> listStaticObj;
	vector<Unit*>listUnit;

	void loadObjectFromFiles();
	void loadContent();
	void loadListObjFromGrid();
	void updateCamera(DWORD dt);
	bool renderBoundingBox;
};

