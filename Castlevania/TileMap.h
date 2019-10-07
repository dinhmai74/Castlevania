#pragma once
#include <vector>
#include "Constants.h"
#include <algorithm>
#include "Debug.h"
#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "TextureManager.h"
#include "SpriteManager.h"

class TileMap
{
	int id;
	LPCWSTR texPath;
	LPCWSTR dataPath;
	int mapWidth;
	int mapHeight;
	int tileWidth;
	int tileHeight;
	int totalRow;
	int totalCol;
	vector<vector<Sprite*>> matrix;
	vector<vector<string>> stringMatrix;
	SpriteManager* sprites;
public:
	TileMap(int id, LPCWSTR texPath, LPCWSTR dataPath, int mapWidth, int mapHeight, int tileWidth, int tileHeight)
		: id(id),
		texPath(texPath),
		dataPath(dataPath),
		mapWidth(mapWidth),
		mapHeight(mapHeight),
		tileWidth(tileWidth),
		tileHeight(tileHeight)
	{
		sprites = SpriteManager::getInstance();
		totalCol = (mapWidth / tileWidth);
		totalRow = (mapHeight / tileHeight);
		init();
	}

	void loadResources();

	void loadMap();

	void draw();

	~TileMap();
	// for update camera base on map
	int getMapWidth() const { return mapWidth; }
	void setMapWidth(int val) { mapWidth = val; }
	int getMapHeight() const { return mapHeight; }
	void setMapHeight(int val) { mapHeight = val; }
	int getTileWidth() const { return tileWidth; }
	void setTileWidth(int val) { tileWidth = val; }
private:
	void init();
};
