﻿#pragma once

#include <vector>
#include "GameObject.h"

#define DEFAULT_CELL_WIDTH		128
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
	D3DXVECTOR2 getUnitPos() { return { x,y }; };
	auto get() { return this->obj; }
};

struct Cell
{
	int row;
	int col;
	Cell& operator =(const Cell& a)
	{
		row = a.row;
		col = a.col;
		return *this;
	}
	bool operator ==(const Cell& a)const
	{
		return row == a.row && col == a.col;
	}
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
	void UnlinkUnit(Unit* unit, int oldRow, int oldCol);
	Cell getCellBaseOnPos(float x, float y);
	int limitRange(int current, int total);
public:
	Grid(int mapWidth, int mapHeight, int cellWidth = DEFAULT_CELL_WIDTH, int cellHeight = DEFAULT_CELL_HEIGHT);
	~Grid();

	void add(Unit* unit);
	void move(Unit* unit, float x, float y); 
	void remove(Unit* unit, float oldRow = -1, float oldCol = -1);
	void get(D3DXVECTOR2 camPosition, vector<Unit*>& listUnits); 
	int getCellWidth() const { return cellWidth; }
	void removeOutOfBoundUnit(Region region);
	void reset();
};
