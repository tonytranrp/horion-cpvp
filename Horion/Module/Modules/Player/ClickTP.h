#pragma once
#include "../Module.h"
class ClickTP : public IModule {
private:
	bool hasClicked = false;
	bool onlyHand = false;
	bool bypass = false;
	bool shouldTP = false;
	Vec3 tpPos;

public:
	ClickTP();
	~ClickTP();

	// Inherited via IModule
	virtual void onTick(GameMode* gm) override;
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
};
