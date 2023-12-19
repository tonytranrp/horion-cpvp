#pragma once
#include "../Module.h"

class AutoMove : public IModule {
private:
	bool sprint = false;
	bool jump = false;

public:
	AutoMove();
	~AutoMove();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
};
