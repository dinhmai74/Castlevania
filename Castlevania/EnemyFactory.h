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
private:
	EnemyFactory() = default;
	static EnemyFactory* instance;
public:
	static int getHp(int enemyType);
};

