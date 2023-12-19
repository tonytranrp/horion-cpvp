#pragma once
#include "../Module.h"
class ToggleSneak : public IModule {
public:
	ToggleSneak();
	~ToggleSneak();

private:
	bool isSneaking = false;
	bool hasPressed = false;

	virtual const char* getModuleName() override;
	void onTick(GameMode* gm);
};
