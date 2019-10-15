#pragma once
#include "Enemy.h"

class EnemyFactory
{
public:
	~EnemyFactory() { instance = nullptr; }

	static EnemyFactory* getInstance() 
	{
		if (instance == nullptr)
			instance = new EnemyFactory;
		return instance;
	}
	Enemy* getEnemy(int type);
	int getHp(int enemyType);
private:
	EnemyFactory() = default;
	static EnemyFactory* instance;
};

