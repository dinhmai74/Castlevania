#include "stage1_castle.h"

Stage1Castle::Stage1Castle()
{
	addAnimation(normal, "stage1_castle_default_ani");
	setType(OBCastle);
}

void Stage1Castle::render()
{
	animations[normal]->render(-1, x, y);
}

