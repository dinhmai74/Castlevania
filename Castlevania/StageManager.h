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
	string getCurrentMapDisplayName() const { return tileMapsInfo[currentStage->getId() - 1].mapDisplayName; }
	void init(vector<TileMapInfo> tileMapsInfo);
	void nextStage(int stageId = -1);
	void reset(int id =-1);
	void setStage(Stage* newStage) { preStage = getCurrentStage(); currentStage = newStage; }
	void render() const
	{
		getCurrentStage()->render();
	}
	void update(const DWORD dt) const { getCurrentStage()->update(dt); }
	void onKeyDown(int keyCode);
	void onKeyUp(int keyCode) const { getCurrentStage()->onKeyUp(keyCode); }
	void keyState(BYTE* states) const { getCurrentStage()->keyState(states); }
	void add(GameObject* ob) const;
	Stage* getCurrentStage() const { return currentStage; }
	void descreaseLife();
private:
	void loadTileMaps();
};
