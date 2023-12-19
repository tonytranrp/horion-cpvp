#pragma once
#include "../Module.h"
class ChestStealer : public IModule {
private:
	int takeDelay = 20;
	bool enhanced = true;
	int closeDelay = 0;

public:
	ChestStealer();
	~ChestStealer();

	// Inherited via IModule
	virtual void ScreenController_tick(ScreenController* c);
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
};
