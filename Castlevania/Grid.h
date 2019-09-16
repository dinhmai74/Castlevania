#pragma once

#include <vector>
#include "GameObject.h"

#define DEFAULT_CELL_WIDTH		256
#define DEFAULT_CELL_HEIGHT		240

class Unit
{
	friend class Grid;

private:
	Grid* grid;
	GameObject* obj;
	float x;
	float y;

	Unit* prev;
	Unit* next;

public:
	Unit(Grid* grid, GameObject* obj, float x, float y);

	void Move(float x, float y);
	auto GetObj() { return this->obj; }
};

class Grid
{
	int mapWidth;
	int mapHeight;

	int cellWidth;
	int cellHeight;

	int numsCol;
	int numsRow;

	vector<vector<Unit*>> cells;

public:
	Grid(int map_width, int map_height, int cell_width = DEFAULT_CELL_WIDTH, int cell_height = DEFAULT_CELL_HEIGHT);
	~Grid();

	void Add(Unit* unit);
	void Move(Unit* unit, float x, float y); // x, y là toạ độ mới của unit, vì vậy lúc này x, y của unit lưu vị trí cũ
	void Get(D3DXVECTOR2 camPosition, vector<Unit*>& listUnits); // lấy tất cả các Unit* nằm trong vùng viewport để Update và Render

	void Out();
};

