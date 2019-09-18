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
	vector<MapGameObjects> getMapSimonCanCollisionObjects();
	void updateGrid();
	void onKeyDown(int keyCode);
	void onKeyUp(int keyCode) const;
	void keyState(BYTE* states) const;
private:
	Simon* simon;
	int mapId;
	Grid* grid;
	wstring mapName;
	vector<GameObject*> listBoundary;
	vector<GameObject*> listItems;
	vector<GameObject*> listCanHitObjects;
	vector<GameObject*> listRenderObj;
	vector<Unit*> listUnit;

	void loadObjectFromFiles();
	void loadContent();
	void loadListObjFromGrid();
	void updateCamera(DWORD dt) const;
	bool renderBoundingBox;
};
