#pragma once
#include "../Module.h"
class Jesus : public IModule {
private:
	bool wasInWater = false;
	bool isBlockBelowWater(Vec3 blockPos);

public:
	bool solid = true;

	Jesus();
	~Jesus();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
