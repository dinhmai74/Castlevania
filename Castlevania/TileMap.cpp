#include "TileMap.h"


void TileMap::loadResources()
{
	auto const texManager = TextureManager::getInstance();
	texManager->add(id, this->texPath, TILEMAP_TRANSPARENT_COLOR);

	auto texTileMap = texManager->get(id);
	// lấy thông tin về kích thước của texture lưu các block tiles (filePath_tex)
	D3DSURFACE_DESC surfaceDesc;
	int level = 0;
	texTileMap->GetLevelDesc(level, &surfaceDesc);

	const int totalTextureRow = surfaceDesc.Height / tileHeight;
	const int totalTextureCol = surfaceDesc.Width / tileWidth;


	// thực hiện lưu danh sách các tile vô sprites theo thứ tự id_sprite
	auto idSprite = 1;

	for (UINT i = 0; i < totalTextureRow; i++)
	{
		for (UINT j = 0; j < totalTextureCol; j++)
		{
			string idTile = "map_" + to_string(id) + "_tile_" + to_string(idSprite);
			int left = tileWidth * j;
			int top = tileHeight * i;
			int right = tileWidth * (j + 1);
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

		// Lưu sprite tile vào vector tilemap
		vector<Sprite*> spriteline;
		stringstream ss(line);
		int n;

		while (ss >> n)
		{
			string idTile = "map_" + to_string(id) + "_tile_" + to_string(n);
			spriteline.push_back(sprites->get(idTile));
		}
		matrix.push_back(spriteline);
	}

	fs.close();
}

void TileMap::draw()
{
	D3DXVECTOR2 camPosition = {0, 0};
	Game::getInstance()->getCameraPosition(camPosition.x, camPosition.y);


	int startRowToDraw = (int)camPosition.y / tileHeight;
	int endRowToDraw = startRowToDraw + SCREEN_HEIGHT / tileHeight;

	int startColToDraw = (int)camPosition.x / tileWidth;
	int endColToDraw = startColToDraw + SCREEN_WIDTH / tileWidth;

	// Xử lí giới hạn
	if (endRowToDraw >= totalRow) endRowToDraw = totalRow - 1;
	if (endColToDraw >= totalCol) endColToDraw = totalCol - 1;

	for (UINT i = startRowToDraw; i <= endRowToDraw; i++)
	{
		for (UINT j = startColToDraw; j <= endColToDraw; j++)
		{
			// +camPosition.x để luôn giữ camera ở chính giữa, vì trong hàm Game::Draw() có trừ cho camPosition.x làm các object đều di chuyển theo
			// +(int)camPosition.x % 32 để giữ cho camera chuyển động mượt
			const auto x = tileWidth * (j - startColToDraw) + camPosition.x - (int)camPosition.x % tileWidth;
			const auto y = tileHeight * (i - startRowToDraw) + camPosition.y - (int)camPosition.y % tileHeight;

			matrix[i][j]->draw(1, x, y + HEADER_HEIGHT);
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
