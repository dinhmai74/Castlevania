/*!
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
	int id;
	std::wstring mapName;
	int mapWidth;
	int mapHeight;
	int tileWidth;
	int tileHeight;
};
