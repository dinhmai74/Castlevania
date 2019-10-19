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
	bool updateEnemy(vector<GameObject*>::value_type obj,DWORD dt);
	void update(DWORD dt);

	void updateSubWeapon(SubWeapon* subWeapon, DWORD dt);

	void onKeyDown(int keyCode);
	void onKeyUp(int keyCode) const;
	void keyState(BYTE* states) const;
	Grid* getGrid() const;
	Simon* getSimon() const;
	int getId();;
	std::wstring getFileObjects() const { return fileObjects; }
	void setFileObjects(std::wstring val) { fileObjects = val; }
	std::wstring getMapName() const { return mapName; }
	void setMapName(std::wstring val) { mapName = val; }
	bool getStopEnemyAction() const { return stopEnemyAction; }
	void setStopEnemyAction(bool val=true) { stopEnemyAction = val; }
	bool isInViewport(GameObject* object);
private:
	Simon* simon;
	int mapId;
	Grid* grid;
	Game* game;
	wstring mapName;
	D3DXVECTOR2 initCam;
	vector<GameObject*> listCanCollideBoundary;
	vector<GameObject*> listDefaultBoundary;
	vector<GameObject*> listItems;
	vector<GameObject*> listCanHitObjects;
	vector<GameObject*> listRenderObj;
	vector<GameObject*> listObjectChangeStage;
	vector<GameObject*> listEnemy;
	vector<GameObject*> listDoor;
	vector<GameObject*> listStopSimObjs;
	vector<GameObject*> listForceIdleSim;
	vector<Unit*> listUnit;
	vector<GameObject*> listStairs;

	void loadObjectFromFiles();
	void loadContent();
	void loadEnemies(fstream &fs, float x, float y);
	void loadBoundaryCase(fstream &fs, float x, float y);
	void initSimonPos();
	void respawnEnemies();
	void loadListObjFromGrid();

	void resetAllList();

	vector<MapGameObjects> getMapSimonCanCollisionObjects();
	void updateCamera(DWORD dt) const;
	void updateInActiveUnit();
	void updateGrid();
	bool renderBoundingBox;
	bool stopEnemyAction;
	wstring fileObjects;
};
