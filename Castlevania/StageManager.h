#pragma once
#include "Stage.h"
#include "TilemapManager.h"

class StageManager
{
	
	static StageManager* instance;
	Stage* currentStage = nullptr;
	Stage* preStage = nullptr;
	vector<TileMapInfo> tileMapsInfo;
public:
	static StageManager* getInstance() {
		if (instance == nullptr) instance = new StageManager();
		return instance;
	};

	~StageManager();

	Stage* getCurrentState() const { return getCurrentStage(); }
	void init(vector<TileMapInfo> tileMapsInfo);
	void setStage(Stage* newStage) { preStage = getCurrentStage(); currentStage = newStage; }
	void render() const
	{
		getCurrentStage()->render();
	}
	void update(const DWORD dt) const { getCurrentStage()->update(dt); }
	void onKeyDown(int keyCode) const { getCurrentStage()->onKeyDown(keyCode); }
	void onKeyUp(int keyCode) const { getCurrentStage()->onKeyUp(keyCode); }
	void keyState(BYTE *states) const { getCurrentStage()->keyState(states); }
	void add(GameObject* ob) const;
	Stage* getCurrentStage() const { return currentStage; }
private:
	void loadTileMaps();
};

