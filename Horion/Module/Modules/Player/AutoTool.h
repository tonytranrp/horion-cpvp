#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"
class AutoTool : public IModule {
public:
	AutoTool();
	~AutoTool();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	int getBestDestoryItemSlot(Block* block);
	virtual void onTick(GameMode* gm) override;
};