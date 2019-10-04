#pragma once
#include "Boundary.h"

class Stage1Castle final : public Boundary
{
public:
	Stage1Castle();
	void render() override;
};
