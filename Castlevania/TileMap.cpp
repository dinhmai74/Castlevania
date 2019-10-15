#include "TileMap.h"

void TileMap::loadResources()
{
	auto const texManager = TextureManager::getInstance();
	texManager->add(id, this->texPath, TILEMAP_TRANSPARENT_COLOR);

	auto texTileMap = texManager->get(id);
	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	texTileMap->GetLevelDesc(level, &surfaceDesc);

	const int totalTextureRow = surfaceDesc.Height / tileHeight;
	const int totalTextureCol = surfaceDesc.Width / getTileWidth();

	auto idSprite = 1;

	for (UINT i = 0; i < totalTextureRow; i++)
	{
		for (UINT j = 0; j < totalTextureCol; j++)
		{
			string idTile = "map_" + to_string(id) + "_tile_" + to_string(idSprite);
			int left = getTileWidth() * j;
			int top = tileHeight * i;
			int right = getTileWidth() * (j + 1);
			int bottom = tileHeight * (i + 1);
			sprites->add(idTile, left, top, right, bottom, left, top, right, bottom, texTileMap);
			idSprite = idSprite + 1;
		}
	}
}

void TileMap::loadMap()
{
	static int c = 0;
	fstream fs;
	fs.open(this->dataPath, ios::in);

	if (fs.fail())
	{
		DebugOut(L"[ERROR] TileMap::Load_MapData failed: ID=%d", id);
		fs.close();
		return;
	}

	string line;

	while (!fs.eof())
	{
		getline(fs, line);
		line.erase(std::remove(line.begin(), line.end(), ','), line.end());

		vector<Sprite*> spriteline;
		vector<string> stringLine;
		stringstream ss(line);
		int n;

		while (ss >> n)
		{
			string idTile = "map_" + to_string(id) + "_tile_" + to_string(n);
			stringLine.push_back(idTile);
		}
		stringMatrix.push_back(stringLine);
	}

	fs.close();
}

void TileMap::draw()
{
	float xCam, yCam;
	Game::getInstance()->getCameraPosition(xCam, yCam);

	auto startColToDraw = static_cast<int>(xCam) / tileWidth;
	auto endColToDraw = static_cast<int>(xCam + SCREEN_WIDTH) / tileWidth;

	int startRowToDraw = (int)yCam / tileHeight;
	int endRowToDraw = startRowToDraw + SCREEN_HEIGHT / tileHeight;

	if (endRowToDraw >= totalRow) endRowToDraw = totalRow - 1;
	if (endColToDraw >= totalCol) endColToDraw = totalCol - 1;

	for (auto i = startRowToDraw; i <= endRowToDraw; i++) {
		for (auto j = startColToDraw; j <= endColToDraw; j++) {
			// get position x,y to draw sprite ( depend on camera  x and y )
			const auto x = tileWidth * (j - startColToDraw) + xCam - static_cast<int>(xCam) % tileWidth;
			const auto y = tileHeight * (i - startRowToDraw) + yCam - static_cast<int>(yCam) % tileHeight;

			sprites->get(stringMatrix[i][j])->draw(-1,x, y+HEADER_HEIGHT);
		}
	}
}

TileMap::~TileMap()
= default;

void TileMap::init()
{
	loadResources();
	loadMap();
}