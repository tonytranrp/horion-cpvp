#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"
class AirJump : public IModule {
private:
	int hasJumped = 0;
	bool legacyMode = false;

public:
	AirJump();
	~AirJump();

	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
};
