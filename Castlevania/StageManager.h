#pragma once
#include "Stage.h"
#include "TilemapManager.h"

class StageManager
{
public:
	static StageManager* getInstance() {
		if (instance == nullptr) instance = new StageManager();
		return instance;
	};

	~StageManager();

	Stage* getCurrentState() const { return getCurrentStage(); }
	string getCurrentMapDisplayName() const { return tileMapsInfo[currentStage->getId() - 1].mapDisplayName; }
	void init(vector<TileMapInfo> tileMapsInfo);
	void nextStage(int stageId = -1, wstring mapName= L"none");
	void reset(int id =-1);
	void setStage(Stage* newStage) { preStage = getCurrentStage(); currentStage = newStage; }
	void render() const;

	void update(const DWORD dt) const;
	void onKeyDown(int keyCode);
	void onKeyUp(int keyCode);
	void keyState(BYTE* states);
	void add(GameObject* ob) const;
	Stage* getCurrentStage() const { return currentStage; }
	void descreaseLife();
	void loadTileMaps();
	void removeAllObjOutOfBound();
	void addScore(int score) { this->score += score; };
	int getScore() { return score; };
	void pauseGame(bool val=true) const;
private:
	int score;
	bool isReleaseSelectMapKey;
	static StageManager* instance;
	Stage* currentStage = nullptr;
	Stage* preStage = nullptr;
	vector<TileMapInfo> tileMapsInfo;
};
