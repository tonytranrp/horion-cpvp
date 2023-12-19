#pragma once
#include "../Module.h"

class StackableItem : public IModule {
public:
	StackableItem();
	~StackableItem();

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
