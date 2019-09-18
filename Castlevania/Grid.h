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

	void move(float x, float y);
	auto get() { return this->obj; }
};

class Grid
{
	int mapWidth;
	int mapHeight;

	int cellWidth;
	int cellHeight;

	int totalCols;
	int totalRows;

	vector<vector<Unit*>> cells;

public:
	Grid(int mapWidth, int mapHeight, int cellWidth = DEFAULT_CELL_WIDTH, int cellHeight = DEFAULT_CELL_HEIGHT);
	~Grid();

	void add(Unit* unit);
	int limitRange(int current, int total);
	void move(Unit* unit, float x, float y); // x, y là toạ độ mới của unit, vì vậy lúc này x, y của unit lưu vị trí cũ
	void get(D3DXVECTOR2 camPosition, vector<Unit*>& listUnits); // lấy tất cả các Unit* nằm trong vùng viewport để Update và Render

};

