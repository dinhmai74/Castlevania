﻿/*!
 * \file tileMapInfo.h
 *
 * \date September 2019
 *
 *
 */

#include <windows.h>
#include <string>

struct TileMapInfo
{
	// id to navigate map
	int id;
	// map name to load map from file
	std::wstring mapObjectsName;
	//  to display HUD
	string mapDisplayName;
	int mapWidth;
	int mapHeight;
	int tileWidth;
	int tileHeight;
};

struct GridInfo {
	wstring gridObjsName;
	int gridWidth;
	int gridHeight;
	int cellWidth;
	int cellHeight;
};
