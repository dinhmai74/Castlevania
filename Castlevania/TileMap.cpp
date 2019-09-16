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
	D3DXVECTOR2 camPosition={0,0};
	Game::getInstance()->getCameraPosition(camPosition.x, camPosition.y);


	int start_row_to_draw = (int)camPosition.y / tileHeight;
	int end_row_to_draw = start_row_to_draw + SCREEN_HEIGHT / tileHeight;

	int start_col_to_draw = (int)camPosition.x / tileWidth;
	int end_col_to_draw = start_col_to_draw + SCREEN_WIDTH / tileWidth;

	// Xử lí giới hạn
	if (end_row_to_draw >= totalRow)
		end_row_to_draw = totalRow- 1;

	if (end_col_to_draw >= totalCol)
		end_col_to_draw = totalCol- 1;

	for (UINT i = start_row_to_draw; i <= end_row_to_draw; i++)
	{
		for (UINT j = start_col_to_draw; j <= end_col_to_draw; j++)
		{
			// +camPosition.x để luôn giữ camera ở chính giữa, vì trong hàm Game::Draw() có trừ cho camPosition.x làm các object đều di chuyển theo
			// +(int)camPosition.x % 32 để giữ cho camera chuyển động mượt
			float x = tileWidth * (j - start_col_to_draw) + camPosition.x - (int)camPosition.x % tileWidth;
			float y = tileHeight * (i - start_row_to_draw) + camPosition.y - (int)camPosition.y % tileHeight;

			matrix[i][j]->draw(1, x, y +HEADER_HEIGHT);
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
