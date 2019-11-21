#pragma once
#include <iostream> 
#include <set> 
#include <iterator> 
#include <Windows.h>
#include "Simon.h"
#include "Boundary.h"
#include "Grid.h"
#include <utility>
#include "TileMapManager.h"
#include "Item.h"
#include "CandleFactory.h"
#include "EnemVampireBoss.h"
#include <string>


class Stage {
public:
	Stage();
	~Stage();

	void init(int mapId, wstring mapName);
	void init(int mapId, wstring mapName, Simon* simon);
	void initMap(int mapId, wstring mapName);
	void initSimon();
	void reset();
	void render();
	bool updateEnemy(vector<GameObject*>::value_type obj, DWORD dt);
	void update(DWORD dt);

	void updateSubWeapon(SubWeapon* subWeapon, DWORD dt);

	void onKeyDown(int keyCode);
	void onKeyUp(int keyCode) const;
	void keyState(BYTE* states) const;

	void removeSubWeapons(GameObject* ob);
	void clearMapByItem();
	void stopEnemyForABit(DWORD time);
	void addSubWeapon(SubWeapon* subWeapon);

	Grid* getGrid() const;
	Simon* getSimon() const;
	int getId();
	std::wstring getFileObjects() const { return fileObjects; }
	void setFileObjects(std::wstring val) { fileObjects = val; }
	std::wstring getMapName() const { return mapName; }
	void setMapName(std::wstring val) { mapName = val; }
	bool getIsGamePause() const { return isGamePause; }
	void setGamePause(bool val = true) { isGamePause = val; }
	bool isInViewport(GameObject* object);
	EnemyVampireBoss* getBoss() const { return boss; }
	void setBoss(EnemyVampireBoss* val) { boss = val; }
	bool getIsFightingBoss() const { return isFightingBoss; }
	void setIsFightingBoss(bool val = true) { isFightingBoss = val; }
	int getCurrentSubWeaponsAmount() { return subWeapons.size(); }
private:
	Simon* simon;
	int mapId;
	bool isFightingBoss;
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
	vector<GameObject*> listBullet;
	vector<GameObject*> subWeapons;
	vector<Unit*> listUnit;
	vector<GameObject*> listStairs;
	EnemyVampireBoss* boss;
	vector<GameObject*> listWater;
	Timer* timerStopEnemy = new Timer();

	void loadObjectFromFiles();
	void loadContent();
	void loadEnemies(fstream& fs, float x, float y);
	void loadBoundaryCase(fstream& fs, float x, float y);
	void respawnEnemies();
	void loadListObjFromGrid();

	void resetAllUnitList();

	vector<MapGameObjects> getMapSimonCanCollisionObjects();
	void updateCamera(DWORD dt) const;
	void updateInActiveUnit();
	void updateGrid();
	bool renderBoundingBox;
	bool isGamePause;
	wstring fileObjects;
	bool isInViewPort(Box pos);
	bool isStopEnemyForDebug;
	void resetAllList();
};
