#pragma once
#include "GameObject.h"

class Stage1Castle final : public GameObject
{
public:
	Stage1Castle();
	~Stage1Castle() = default;
	void initAnim() override;
};
