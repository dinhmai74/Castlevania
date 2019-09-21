#pragma once
#include <Windows.h>
#include "Simon.h"
#include "Boundary.h"
#include "Grid.h"
#include <utility>
#include "TileMapManager.h"
#include "Item.h"
#include "CandleFactory.h"


class Stage
{
public:
	Stage();
	~Stage();

	void init(int mapId, wstring mapName);
	void render();
	void update(DWORD dt);
	void onKeyDown(int keyCode);
	void onKeyUp(int keyCode) const;
	void keyState(BYTE* states) const;
	Grid* getGrid() const { return grid; }
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
	void initSimonPos();
	void loadListObjFromGrid();
	vector<MapGameObjects> getMapSimonCanCollisionObjects();
	void updateCamera(DWORD dt) const;
	bool isInViewport(GameObject* object);
	void updateInActiveUnit();
	void updateGrid();
	bool renderBoundingBox;
};

