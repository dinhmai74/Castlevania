#include "TileMapManager.h"

TileMapManager* TileMapManager::instance = NULL;

void TileMapManager::add(int ID, LPCWSTR filePathTex, LPCWSTR filePathData, int mapWidth, int mapHeight, int tileWidth /*= 32*/, int tileHeight /*= 32*/)
{
	auto tilemap = new TileMap(ID, filePathTex, filePathData, mapWidth, mapHeight, tileWidth, tileHeight);
	tilemaps[ID] = tilemap;
}

void TileMapManager::add(int ID, wstring mapName, int mapWidth, int mapHeight, int tileWidth /*= 32*/, int tileHeight /*= 32*/)
{
	std::wstring mapTex = STAGE_PREFIX_PATH + mapName + L".BMP";
	std::wstring mapData = STAGE_PREFIX_PATH + mapName + STAGE_MAP_PATH;
	add(ID, mapTex.c_str(), mapData.c_str(), mapWidth, mapHeight, tileWidth, tileHeight);
}

void TileMapManager::add(TileMapInfo mapInfo)
{
	add(mapInfo.id, mapInfo.mapObjectsName, mapInfo.mapWidth, mapInfo.mapHeight, mapInfo.tileWidth, mapInfo.tileHeight);
}