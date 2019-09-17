#pragma once
#include "Item.h"

constexpr auto WHIP_ITEM_W = 32;
constexpr auto WHIP_ITEM_H = 32;

class WhipItem :
	public Item
{
public:
	WhipItem();
	~WhipItem() = default;

	void update(DWORD dt, vector<LPGameObject>* bricks) override;
	void render() override;

	virtual Box getBoundingBox() override;

};

