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

	void goToStage(int mapId, wstring mapName);
	void init(int mapId, wstring mapName);
	void init(int mapId, wstring mapName, Simon* simon);
	void initMap(int mapId, wstring mapName);
	void initSimon();
	void reset();
	void render();
	void update(DWORD dt);

	void updateSubWeapon(SubWeapon* subWeapon, DWORD dt);

	void onKeyDown(int keyCode);
	void onKeyUp(int keyCode) const;
	void keyState(BYTE* states) const;
	Grid* getGrid() const { return grid; }
	Simon* getSimon() const { return simon; }
	int getId() { return mapId; };
private:
	Simon* simon;
	int mapId;
	Grid* grid;
	wstring mapName;
	D3DXVECTOR2 initCam;
	vector<GameObject*> listBoundary;
	vector<GameObject*> listItems;
	vector<GameObject*> listCanHitObjects;
	vector<GameObject*> listRenderObj;
	vector<GameObject*> listObjectChangeStage;
	vector<GameObject*> listEnemy;
	vector<Unit*> listUnit;
	vector<GameObject*> listStairs;

	void loadObjectFromFiles();
	void loadContent();
	void initSimonPos();
	void respawnEnemies();
	void loadListObjFromGrid();
	vector<MapGameObjects> getMapSimonCanCollisionObjects();
	void updateCamera(DWORD dt) const;
	bool isInViewport(GameObject* object);
	void updateInActiveUnit();
	void updateGrid();
	bool renderBoundingBox;
	bool stopEnemyAction;
};
