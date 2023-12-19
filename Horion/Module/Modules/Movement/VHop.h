#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"
class VHop : public IModule {
private:
	int stage, warmups = 2;
	float moveSpeed, lastMoveSpeed;

	bool weirdFriction = false;
	float theSpeed = 1.74f;

public:
	VHop();
	~VHop();

	virtual const char* getModuleName() override;
	virtual void onMove(MoveInputHandler* input) override;
	virtual void onDisable();
	virtual void onEnable();
};
