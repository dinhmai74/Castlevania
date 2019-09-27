#include "SweptAABB.h"
#include <algorithm>
using namespace std;

bool isColliding(float bl, float bt, float br, float bb, float sl, float st, float sr, float sb)
{
	return !(br < sl || bl > sr || bb < st || bt > sb);
}

bool isColliding(const Box box1, const Box box2)
{
	return isColliding(box1.l, box1.t, box1.r, box1.b, box2.l, box2.t, box2.r, box2.b);
}

void sweptAABB(
	float ml, // move left
	float mt, // move top
	float mr, // move right
	float mb, // move bottom
	float sl, // static left
	float st,
	float sr,
	float sb,
	float dx, float dy,
	float& t, float& nx, float& ny)
{
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f; // no collision
	nx = ny = 0;

	//
	// Broad-phase test
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;

	if (dx == 0 && dy == 0) return; // moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}

	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -99999999999;
		tx_exit = 99999999999;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999;
		ty_exit = 99999999999;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}

	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

void sweptAABB(Box movingBox, Box staticBox, float dx, float dy, float& t, float& nx, float& ny)
{
	return sweptAABB(movingBox.l, movingBox.t, movingBox.r, movingBox.b,
		staticBox.l, staticBox.t, staticBox.r, staticBox.b,
		dx, dy, t, nx, ny
	);
}