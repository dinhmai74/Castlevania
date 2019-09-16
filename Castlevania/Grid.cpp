#include "Grid.h"

Unit::Unit(Grid* grid, LPGAMEOBJECT obj, float x, float y)
{
	this->grid = grid;
	this->obj = obj;
	this->x = x;
	this->y = y;

	this->prev = NULL;
	this->next = NULL;

	grid->Add(this);
}

void Unit::Move(float x, float y)
{
	grid->Move(this, x, y);
}

Grid::Grid(int map_width, int map_height, int cell_width, int cell_height)
{
	this->mapWidth = map_width;
	this->mapHeight = map_height;

	this->cellWidth = cell_width;
	this->cellHeight = cell_height;

	numsCol = map_width / cell_width;
	numsRow = map_height / cell_height;

	cells.resize(numsRow);

	for (int i = 0; i < numsRow; i++)
		cells[i].resize(numsCol);

	for (int i = 0; i < numsRow; i++)
		for (int j = 0; j < numsCol; j++)
			cells[i][j] = NULL;
}

Grid::~Grid()
{
}

void Grid::Add(Unit* unit)
{
	int row = (int)(unit->y / cellHeight);
	int col = (int)(unit->x / cellWidth);

	// thêm vào đầu cell - add head
	unit->prev = NULL;
	unit->next = cells[row][col];
	cells[row][col] = unit;

	if (unit->next != NULL)
		unit->next->prev = unit;
}

void Grid::Move(Unit* unit, float x, float y)
{
	// lấy chỉ số cell cũ
	int old_row = (int)(unit->y / cellHeight);
	int old_col = (int)(unit->x / cellWidth);

	// lấy chỉ số cell mới
	int new_row = (int)(y / cellHeight);
	int new_col = (int)(x / cellWidth);

	// nếu object ra khỏi vùng viewport -> không cần cập nhật
	if (new_row < 0 || new_row >= numsRow || new_col < 0 || new_col >= numsCol)
		return;

	// cập nhật toạ độ mới
	unit->x = x;
	unit->y = y;

	// cell không thay đổi
	if (old_row == new_row && old_col == new_col)
		return;

	// huỷ liên kết với cell cũ
	if (unit->prev != NULL)
	{
		unit->prev->next = unit->next;
	}

	if (unit->next != NULL)
	{
		unit->next->prev = unit->prev;
	}

	if (cells[old_row][old_col] == unit)
	{
		cells[old_row][old_col] = unit->next;
	}

	// thêm vào cell mới
	Add(unit);
}

void Grid::Get(D3DXVECTOR2 camPosition, vector<Unit*>& listUnits)
{
	int start_col = (int)(camPosition.x / cellWidth);
	int end_col = ceil((camPosition.x + SCREEN_WIDTH) / cellWidth);

	for (int i = 0; i < numsRow; i++)
	{
		for (int j = start_col; j < end_col; j++)
		{
			Unit* unit = cells[i][j];

			while (unit != NULL)
			{
				if (unit->GetObj()->IsEnable() == true)
					listUnits.push_back(unit);

				unit = unit->next;
			}
		}
	}
}

void Grid::Out()
{
	for (int i = 0; i < numsRow; i++)
	{
		for (int j = 0; j < numsCol; j++)
		{
			int c = 0;
			Unit* unit = cells[i][j];

			while (unit)
			{
				c++;
				unit = unit->next;
			}

			DebugOut(L"%d\t", c);
		}

		DebugOut(L"\n");
	}
}


