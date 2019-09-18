#include "Grid.h"

Unit::Unit(Grid* grid, LPGAMEOBJECT obj, float x, float y)
{
	this->grid = grid;
	this->obj = obj;
	this->x = x;
	this->y = y;

	this->prev = nullptr;
	this->next = nullptr;

	grid->add(this);
}

void Unit::move(float x, float y)
{
	grid->move(this, x, y);
}

Grid::Grid(int mapWidth, int mapHeight, int cellWidth, int cellHeight)
{
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;

	this->cellWidth = cellWidth;
	this->cellHeight = cellHeight;

	totalCols = mapWidth / cellWidth;
	totalRows = mapHeight / cellHeight;

	cells.resize(totalRows);

	for (auto i = 0; i < totalRows; i++)
		cells[i].resize(totalCols);

	for (auto i = 0; i < totalRows; i++)
		for (auto j = 0; j < totalCols; j++)
			cells[i][j] = nullptr;
}

Grid::~Grid()
{
}

void Grid::add(Unit* unit)
{
	// dua vao x,y -> calculate row and and unit in
	auto row = static_cast<int>(unit->y / cellHeight);
	auto col = static_cast<int>(unit->x / cellWidth);

	// validate if the case out of range;
	row = limitRange(row, totalRows);
	col = limitRange(col, totalCols);

	// thêm vào đầu cell - add head
	unit->prev = nullptr;
	unit->next = cells[row][col];
	cells[row][col] = unit;

	if (unit->next != nullptr)
		unit->next->prev = unit;
}

int Grid::limitRange(int current, int total)
{
	if (current < 0) return 0;
	if (current >= total) return total - 1;
	return current;
}

void Grid::move(Unit* unit, float x, float y)
{
	const auto oldRow = static_cast<int>(unit->y / cellHeight);
	const auto oldCol = static_cast<int>(unit->x / cellWidth);

	const auto newRow = static_cast<int>(y / cellHeight);
	const auto newCol = static_cast<int>(x / cellWidth);

	// nếu object ra khỏi vùng viewport -> không cần cập nhật
	if (newRow < 0 || newRow >= totalRows || newCol < 0 || newCol >= totalCols)
		return;

	// cập nhật toạ độ mới
	unit->x = x;
	unit->y = y;

	// cell không thay đổi
	if (oldRow == newRow && oldCol == newCol)
		return;

	// huỷ liên kết với cell cũ
	if (unit->prev != nullptr)
		unit->prev->next = unit->next;

	if (unit->next != nullptr)
		unit->next->prev = unit->prev;

	if (cells[oldRow][oldCol] == unit)
		cells[oldRow][oldCol] = unit->next;

	// thêm vào cell mới
	add(unit);
}

void Grid::get(D3DXVECTOR2 camPosition, vector<Unit*>& listUnits)
{
	const auto startCol = static_cast<int>(camPosition.x / cellWidth);
	const int endCol = ceil((camPosition.x + SCREEN_WIDTH) / cellWidth);

	for (auto i = 0; i < totalRows; i++)
	{
		for (int j = startCol; j < endCol; j++)
		{
			auto unit = cells[i][j];

			while (unit != nullptr)
			{
				if (unit->get()->IsEnable())
					listUnits.push_back(unit);

				unit = unit->next;
			}
		}
	}
}

