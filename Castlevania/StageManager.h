#pragma once
#include "Stage.h"
#include "TilemapManager.h"
#include <unordered_map>

enum EndGameStatus {
	EndGameNone,
	EndGameStart,
	EndGameAddHp,
	EndGameTimeToScore, // 1s=10 score
	EndGameHeartToScore, // 1h = 100 score
	EndGameDone
};

class StageManager {
public:
	static StageManager* getInstance() {
		if (instance == nullptr) instance = new StageManager();
		return instance;
	};

	~StageManager();

	string getCurrentMapDisplayName() const { return tileMapsInfo[currentStage->getId() - 1].mapDisplayName; }
	void init(vector<TileMapInfo> tileMapsInfo, vector<GridInfo> gridsInfo);

	void loadObjectsFromFile(wstring mapObjsName);
	void loadBoundaries(fstream& fs, float x, float y, wstring mapObjsName);
	void loadEnemies(fstream& fs, float x, float y, wstring mapObjsName);
	void nextStage(int stageId = -1, wstring mapName = L"none");
	void resetStage(int id = -1, wstring mapName = L"none");
	void setStage(Stage* newStage) { preStage = getCurrentStage(); currentStage = newStage; }
	void render();

	void updateEndGame();
	void update(const DWORD dt);
	void onKeyDown(int keyCode);
	void onKeyUp(int keyCode);
	void keyState(BYTE* states);
	void add(GameObject* ob) const;
	void add(GameObject* ob, float x, float y, wstring mapName) const;
	Stage* getCurrentStage() const { return currentStage; }
	void descreaseLife();
	void loadTileMaps();
	void removeAllObjOutOfBound();
	void addScore(int score) { this->score += score; }
	void addSubWeapon(SubWeapon* subWeapon);
	void setEndGame();
	int getRemainTime() { return defaultTime - time / CLOCKS_PER_SEC; };
	void removeSubWeapon(GameObject* sub) { currentStage->removeSubWeapons(sub); }
	int getCurrentSubWeaponsAmount() { return currentStage->getCurrentSubWeaponsAmount(); }
	int getScore() { return score; };
	void pauseGame(bool val = true) const;
	void setCheckPoint(CheckPoint val) { checkPoint = val; }

	void clearMapByItem();
	void stopEnemyForABit(DWORD time = 2000);
	bool getIsWhipMaxLv();
	bool getIsGameOver() const { return isGameOver; }
	void setIsGameOver(bool val) { isGameOver = val; }
	int getPlayerChoseWhenOver() const { return playerChoseWhenOver; }
	void setPlayerChoseWhenOver(int val) { playerChoseWhenOver = val; }
	int getSceneId() const { return isStartPlaying; }
	void setPlaying(int val) {
		isStartPlaying = val;
	}
	void resetGame();
	std::unordered_map<std::wstring, Grid*> getGrids() const { return grids; }
	void setGrids(std::unordered_map<std::wstring, Grid*> val) { grids = val; }
	void doThunderEffect();
	bool isDoingThunderEffect() { return timerThunderEffect->isRunning(); }
private:
	int isStartPlaying;
	int score;
	bool isReleaseSelectMapKey;
	unordered_map<wstring, Grid*> grids;

	int endGameState;
	Timer* timerCountHeart = new Timer(100);
	int defaultTime;
	int time;
	DWORD dt;
	Timer* timerEndGame = new Timer(1000);
	Timer* timerThunderEffect = new Timer(500);
	static StageManager* instance;
	Stage* currentStage = nullptr;
	Stage* preStage = nullptr;
	vector<TileMapInfo> tileMapsInfo;
	vector<GridInfo> gridsInfo;
	CheckPoint checkPoint;
	bool isGameOver;
	int playerChoseWhenOver;
	void loadGrids();
	bool introDone;
public:
	bool getPlaying();
};
