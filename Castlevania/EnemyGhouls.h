#pragma once
#include "Enemy.h"

class EnemyGhouls final : public Enemy
{
public:
	EnemyGhouls();
	~EnemyGhouls();

	void init();
	void initAnim() override;

protected:
	
private:
};
