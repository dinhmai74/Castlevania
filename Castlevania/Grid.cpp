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

void Grid::UnlinkUnit(Unit* unit, int oldRow, int oldCol)
{
	// unlink the old cell contain unit
	if (unit->prev != nullptr)
		unit->prev->next = unit->next;

	if (unit->next != nullptr)
		unit->next->prev = unit->prev;

	if (cells[oldRow][oldCol] == unit)
		cells[oldRow][oldCol] = unit->next;
}

Cell Grid::getCellBaseOnPos(float x, float y)
{
	auto row = static_cast<int>(y / cellHeight);
	auto col = static_cast<int>(x / cellWidth);

	return { row,col };
}

void Grid::move(Unit* unit, float x, float y)
{
	const auto oldCell = getCellBaseOnPos(unit->x, unit->y);
	const auto newCell = getCellBaseOnPos(x, y);

	const auto newRow = newCell.row;
	const auto newCol = newCell.col;
	const auto oldCol = oldCell.col;
	const auto oldRow = oldCell.row;

	//// if out boundary => we won't care about that unit
	if (newRow < 0 || newRow >= totalRows || newCol < 0 || newCol >= totalCols)
		return;
	// if still in the same cell => not update;
	unit->x = x;
	unit->y = y;

	if (oldRow == newRow && oldCol == newCol)return;

	remove(unit, oldRow, oldCol);

	add(unit);
}

void Grid::remove(Unit* unit, float oldRow, float oldCol)
{
	auto cell = getCellBaseOnPos(unit->x, unit->y);
	if (oldCol == -1) oldCol = cell.col;
	if (oldRow == -1) oldRow = cell.row;
	// unlink the old cell contain unit
	UnlinkUnit(unit, oldRow, oldCol);
}

void Grid::get(D3DXVECTOR2 camPosition, vector<Unit*>& listUnits)
{
	const auto startCol = static_cast<int>(camPosition.x / cellWidth);
	const int endCol = ceil((camPosition.x + SCREEN_WIDTH) / cellWidth);

	for (auto i = 0; i < totalRows; i++)
	{
		for (auto j = startCol; j < endCol; j++)
		{
			auto unit = cells[i][j];

			while (unit != nullptr)
			{
				if (unit->get()->IsActive())
					listUnits.push_back(unit);

				unit = unit->next;
			}
		}
	}
}
