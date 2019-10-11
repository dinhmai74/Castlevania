#include "stage1_castle.h"

Stage1Castle::Stage1Castle()
{
	initAnim();
	setAnimId(normal);
	setType(OBCastle);
}

void Stage1Castle::initAnim()
{
	addAnimation(normal, "stage1_castle_default_ani");
}

