#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class FastStop : public IModule {
private:

public:
	FastStop();
	~FastStop();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onMove(MoveInputHandler* input) override;
};
