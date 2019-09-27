#pragma once
#include <unordered_map>
#include <Windows.h>
#include "TileMap.h"
#include "tileMapInfo.h"

class TileMapManager
{
public:
	~TileMapManager() { instance = nullptr; }

	static TileMapManager* getInstance()
	{
		if (instance == nullptr)
			instance = new TileMapManager;
		return instance;
	}
	void add(int ID, LPCWSTR filePathTex, LPCWSTR filePathData, int mapWidth, int mapHeight, int tileWidth = 32, int tileHeight = 32);
	void add(int ID, wstring mapName, int mapWidth, int mapHeight, int tileWidth = 32, int tileHeight = 32);
	void add(TileMapInfo mapInfo);
	auto get(int ID) { return tilemaps[ID]; }
private:
	std::unordered_map<int, TileMap*> tilemaps;
	TileMapManager() = default;
	static TileMapManager* instance;
};
